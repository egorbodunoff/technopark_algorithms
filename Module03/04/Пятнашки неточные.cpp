#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>

const int SideSize = 4;
const int FieldSize = SideSize * SideSize;
const std::array<char, FieldSize> FinishField({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0});

class GameState {
public:
    GameState(const std::array<char, FieldSize> &_field);

    int Heuristic() const;
    bool CanMoveLeft() const;
    bool CanMoveUp() const;
    bool CanMoveRight() const;
    bool CanMoveDown() const;

    GameState MoveLeft() const;
    GameState MoveUp() const;
    GameState MoveRight() const;
    GameState MoveDown() const;

    bool IsFinish() const;

    char GetFieldValue(std::size_t pos) const;

    bool operator==(const GameState &state) const { return field == state.field; }
    bool IsSolvable() const;

private:
	std::array<char, FieldSize> field;
	int zeroPos;
};


GameState::GameState(const std::array<char, FieldSize> &_field) : field(_field), zeroPos(-1) {
    for (std::size_t i = 0; i < field.size(); ++i) {
        if (field[i] == 0) {
            zeroPos = i;
            break;
        }
    }
    assert(zeroPos != -1);
}

int GameState::Heuristic() const {
    int h = 0;
    for (int i = 0; i < FieldSize; ++i) {
        if (field[i] && field[i] != i + 1) {
            h += std::abs(i / SideSize - (field[i] - 1) / SideSize);
            h += std::abs(i % SideSize - (field[i] - 1) % SideSize);
        }
    }
    return h;
}

bool GameState::CanMoveLeft() const { return zeroPos % SideSize != 0; }
bool GameState::CanMoveUp() const { return zeroPos >= SideSize; }
bool GameState::CanMoveRight() const { return zeroPos % SideSize < SideSize - 1; }
bool GameState::CanMoveDown() const { return zeroPos < FieldSize - SideSize; }

GameState GameState::MoveLeft() const {
    assert(CanMoveLeft());
    GameState next(*this);
    std::swap(next.field[zeroPos], next.field[zeroPos - 1]);
    --next.zeroPos;
    return next;
}

GameState GameState::MoveUp() const {
    assert(CanMoveUp());
    GameState next(*this);
    std::swap(next.field[zeroPos], next.field[zeroPos - SideSize]);
    next.zeroPos -= SideSize;
    return next;
}

GameState GameState::MoveRight() const {
    assert(CanMoveRight());
    GameState next(*this);
    std::swap(next.field[zeroPos], next.field[zeroPos + 1]);
    ++next.zeroPos;
    return next;
}

GameState GameState::MoveDown() const {
    assert(CanMoveDown());
    GameState next(*this);
    std::swap(next.field[zeroPos], next.field[zeroPos + SideSize]);
    next.zeroPos += SideSize;
    return next;
}

bool GameState::IsFinish() const { return field == FinishField; }

char GameState::GetFieldValue(std::size_t pos) const {
    assert(pos >= 0 && pos < FieldSize);
    return field[pos];
}

bool GameState::IsSolvable() const {
    int invs = 0;
    for (int i = 0; i < FieldSize - 1; ++i) {
        for (int j = i + 1; j < FieldSize; ++j) {
            if (field[i] > field[j] && field[i] && field[j]) {
                invs++;
            }
        }
    }
    return (invs + zeroPos / SideSize + 1) % 2 == 0;
}

struct Hasher {
    std::size_t operator()(const GameState &state) const {
        std::size_t hash = 0;
        std::size_t p = 31;
        for (int i = 0; i < FieldSize; ++i) {
            p *= 31;
            hash += state.GetFieldValue(i) * p;
        }
        return hash;
    }
};

struct State {
    GameState state;
    int moves;
    State(const GameState &_state, int _moves) : state(_state), moves(_moves) {}

    bool operator<(const State &other) const {
        if (moves == other.moves) {
            return state.Heuristic() < other.state.Heuristic();
        }
        return moves < other.moves;
    }

    bool operator==(const State &other) const {
        if (moves == other.moves) {
            return state == other.state;
        }
        return moves == other.moves;
    }
};

void MakeMove(std::unordered_map<GameState, char, Hasher> &visited, std::set<State> &p_que,
              GameState &current) {
    if (current.CanMoveLeft()) {
        GameState next = current.MoveLeft();
        if (visited.find(next) == visited.end()) {
            visited[next] = 'R';
            p_que.insert(State(next, next.Heuristic()));
        }
    }
    if (current.CanMoveUp()) {
        GameState next = current.MoveUp();
        if (visited.find(next) == visited.end()) {
            visited[next] = 'D';
            p_que.insert(State(next, next.Heuristic()));
        }
    }
    if (current.CanMoveRight()) {
        GameState next = current.MoveRight();
        if (visited.find(next) == visited.end()) {
            visited[next] = 'L';
            p_que.insert(State(next, next.Heuristic()));
        }
    }
    if (current.CanMoveDown()) {
        GameState next = current.MoveDown();
        if (visited.find(next) == visited.end()) {
            visited[next] = 'U';
            p_que.insert(State(next, next.Heuristic()));
        }
    }
}

std::string BuildRoute(std::unordered_map<GameState, char, Hasher> &visited) {
    std::string route;
    GameState state(FinishField);
    char move = visited[state];
    while (move != 'S') {
        switch (move) {
            case 'R':
                state = state.MoveRight();
                break;
            case 'D':
                state = state.MoveDown();
                break;
            case 'L':
                state = state.MoveLeft();
                break;
            case 'U':
                state = state.MoveUp();
                break;
        }
        route += move;
        move = visited[state];
    }
    std::reverse(route.begin(), route.end());
    return route;
}

std::string GetSolution(const GameState &state) {
    std::unordered_map<GameState, char, Hasher> visited;
    visited[state] = 'S';
    std::set<State> p_que;
    p_que.insert(State(state, state.Heuristic()));
    while (!p_que.empty()) {
        auto top = p_que.begin();
        GameState current = (*top).state;
        p_que.erase(top);
        if (current.IsFinish()) {
            break;
        }
        MakeMove(visited, p_que, current);
    }
    return BuildRoute(visited);
}

int main() {
    std::array<char, FieldSize> start;
    int fieldValue = 0;
    for (int i = 0; i < FieldSize; ++i) {
        std::cin >> fieldValue;
        start[i] = fieldValue;
    }
    GameState state(start);
    if (!state.IsSolvable()) {
        std::cout << -1;
        return 0;
    }
    std::string answer = GetSolution(state);
    std::cout << answer.size() << std::endl << answer;
    return 0;
}
