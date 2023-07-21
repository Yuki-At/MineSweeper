#include "Field.h"

#include <algorithm>
#include <cassert>
#include <functional>
#include <random>

namespace {


    // Generate number of N random, unique numbers from 0 to R - 1 without X.
    std::vector<int> GenerateRandomUniqueNumbers(int R, int N, std::vector<int> X) {
        assert(static_cast<std::size_t>(R) > static_cast<std::size_t>(N) + X.size());

        std::random_device seedGen;
        std::mt19937 engine(seedGen());

        std::vector<int> pool(R-1);
        std::vector<int> result;

        {
            int i = 0;
            std::ranges::for_each(pool, [&](auto & p) { p = i++; });
        }

        // Sort X and remove duplicate numbers.
        std::ranges::sort(X);
        std::ranges::unique(X);

        // Remove the numbers of X from pool.
        for (auto itr = pool.rbegin(), end = pool.rend(); itr != end; ++itr) {
            if (*itr == X.back()) {
                pool.erase(std::next(itr).base());
                X.pop_back();
            }
        }

        while (result.size() < static_cast<std::size_t>(N)) {
            const auto i = engine() % pool.size();

            result.push_back(pool[i]);
            pool.erase(pool.begin() + i);
        }

        std::ranges::sort(result);
        return result;
    }

}

void Field::BuryMines(int N, std::vector<Pos> X) {
    Clear();
    buriedMineCount = N;

    std::vector<int> bannedNumbers;

    bannedNumbers.reserve(X.size());
    std::ranges::transform(X, std::back_inserter(bannedNumbers), std::bind(GetIndex, this, std::placeholders::_1));

    const auto indices = GenerateRandomUniqueNumbers(GetGridCount(), N, bannedNumbers);

    for (const auto index : indices) {
        grids[index].hasMine = true;
    }
}

bool Field::OpenGrid(Pos pos) {
    if (GetGrid(pos).guessFlag) {
        return true;
    }

    if (GetGrid(pos).hasMine) {
        return false;
    }

    const std::function<void(Pos)> fn = [&](const Pos & pos) {
        if (GetGrid(pos).open) {
            return;
        }

        int aroundMineCount = 0;
        for (const auto & p : pos.GetSurroundingPos()) {
            if (this->IsExist(p)) {
                aroundMineCount += this->GetGrid(p).hasMine;
            }
        }

        this->grids[this->GetIndex(pos)].open = true;
        this->grids[this->GetIndex(pos)].aroundMineCount = aroundMineCount;

        if (aroundMineCount > 0) {
            return;
        }

        for (const auto & p : pos.GetSurroundingPos()) {
            if (this->IsExist(p)) {
                fn(p);
            }
        }
    };

    fn(pos);

    return true;
}
