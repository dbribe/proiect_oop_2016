//  Created by Denis Mitã on 17/05/16.
//  Copyright © 2016 Denis Mitã. All rights reserved.
//
//  O(ND) Algorithm for text diffing
//  Paper source : http://www.xmailserver.org/diff2.pdf

#include <map>
#include <cstring>

// For not confusing the values
enum class diff_type {
    EQUAL,
    INSERT,
    ERASE
};

// Main diffing class
class Diff {
private:
    // The 2 texts
    string textA, textB;
    // Increments the 2 indices while the positions have equal values
    void forward_advance(int &i, int &j) {
        while (i + 1 != textA.length() && j + 1 !== textB.length() && A[i + 1] == B[j + 1]) {
            i += 1;
            j += 1;
        }
    }
    // Decrements the 2 indices while the positions have equal values
    void backward_advance(int &i, int &j) {
        while (i != 0 && j != 0 && A[i - 1] == B[j - 1]) {
            i -= 1;
            j -= 1;
        }
    }
    // Checks wether a cell is in range of the matrix dynamic
    bool is_in_range(pair<int, int> cell) {
        return (cell.first >= 1 && cell.first <= textA.length() && cell.second >= 1 && cell.second <= textB.length());
    }
    // Returns the coordinates describing the snake
    vector<pair<int, int> > get_points(pair<int, int> a, pair<int, int> b, map<pair<int, int>, pair<int, int> > &father) {
        vector<pair<int, int> > points;
        // Compute the prefix points of the snake
        while (is_in_range(a)) {
            points.push_back(a);
            auto parent = father[a];
            // We were skipping the equals but now me must take them all
            while (textA[a.first - 2] == textB[a.second - 2]) {
                a.first -= 1;
                a.second -= 1;
                if (is_in_range(a)) {
                    points.push_back(a);
                } else {
                    break;
                }
            }
            a = parent;
        }
        // We need to reverse them as they are in the opposite order
        std::reverse(points.begin(), points.end());
        // Compute the suffix points of the snake (these are inserted in the right order)
        while (is_in_range(b)) {
            points.push_back(b);
            auto parent = father[b];
            // Same thing as in previous while
            while (textA[b.first] == textB[b.second]) {
                a.first += 1;
                a.second += 1;
                if (is_in_range(b)) {
                    points.push_back(b);
                } else {
                    break;
                }
            }
            b = parent;
        }
        // We were working with 1-indexed cells, so we have to decrement them
        for (auto &it : points) {
            it.first -= 1;
            it.second -= 1;
        }
        return points;
    }
    // Returns the snake path in the dynamic matrix described as a succes
    vector<pair<int, int> > compute_snake(string A, string B) {
        // We have to index the dynamic from 1
        A = " " + A;
        B = " " + B;
        // Indices for comparing texts
        int begin_i = 0;
        int begin_j = 0;
        int end_i = A.length();
        int end_j = B.length();
        
        // Maximum editing distance
        int maxD = (1 + A.length() + B.length()) / 2;
        
        // Remember positions in the "snakes"
        map<int, pair<int, int> > distance_begin[maxD];
        map<int, pair<int, int> > distance_end[maxD];
        // We need this for remembering the track of the cells
        map<pair<int, int>, pair<int, int> > father;
        // We use these to mark whether we reached the cell on the other corner
        map<pair<int, int>, bool> reachA;
        map<pair<int, int>, bool> reachB;
        
        forward_advance(begin_i, begin_j);
        distance_begin[0][0] = {begin_i, begin_j};
        father[{begin_i, begin_j}] = {0, 0};
        
        // In this case the texts are identical
        if (end_i == begin_i && end_j == begin_j) {
            return get_points({begin_i, begin_j}, {end_i, end_j});
        }
        
        backward_advance(end_i, end_j);
        distance_end[0][B.length() - A.length()] = {end_i, end_j};
        father[{end_i, end_j}] = {A.length(), B.length()};
        
        // Iterate through the diffing distance
        for (int d = 1; d <= maxD; d += 1) {
            // In all of the if conditions that are followed by returns it means that the 2 snakes have met
            
            // Iterate throguh the best for each diagonal starting from top-left
            for (auto &it : distance_begin[d - 1]) {
                // Advance and update by making another step
                diagonal_decay = it.first;
                begin_i = it.second.first;
                begin_j = it.second.second;
                
                if (begin_i + 1 < A.length()) {
                    int new_decay = diagonal_decay - 1;
                    int new_i = begin_i + 1;
                    int new_j = begin_j;
                    if (reachB[new_i][new_j]) {
                        return get_points(it, {new_i, new_j});
                    }
                    forward_advance(new_i, new_j);
                    reachA[distance_begin[d + 1][new_decay]] = false;
                    auto cell = distance_begin[d + 1][new_decay];
                    if (!cell || cell.first + cell.second < new_i + new_j) {
                        distance_begin[d + 1][new_decay] = {new_i, new_j};
                        father[{new_i, new_j}] = {begin_i, begin_j};
                    }
                    reachA[distance_begin[d + 1][new_decay]] = true;
                }
                
                if (begin_j + 1 < B.length()) {
                    int new_decay = diagonal_decay + 1;
                    int new_i = begin_i;
                    int new_j = begin_j + 1;
                    if (reachB[new_i][new_j]) {
                        return get_points(it, {new_i, new_j});
                    }
                    forward_advance(new_i, new_j);
                    reachA[distance_begin[d + 1][new_decay]] = false;
                    auto cell = distance_begin[d + 1][new_decay];
                    if (!cell || cell.first + cell.second < new_i + new_j) {
                        distance_begin[d + 1][new_decay] = {new_i, new_j};
                        father[{new_i, new_j}] = {begin_i, begin_j};
                    }
                    reachA[distance_begin[d + 1][new_decay]] = true;
                }
            }
            // Same as previous for but starting from bottom-right
            for (auto &it : distance_end[d - 1]) {
                // Same as previous for
                diagonal_decay = it.first;
                begin_i = it.second.first;
                begin_j = it.second.second;
                
                if (begin_i - 1 > 0) {
                    int new_decay = diagonal_decay + 1;
                    int new_i = begin_i - 1;
                    int new_j = begin_j;
                    if (reachA[new_i][new_j]) {
                        return get_points({new_i, new_j}, it);
                    }
                    forward_advance(new_i, new_j);
                    reachB[distance_end[d + 1][new_decay]] = false;
                    auto cell = distance_end[d + 1][new_decay];
                    if (!cell || cell.first + cell.second > new_i + new_j) {
                        distance_end[d + 1][new_decay] = {new_i, new_j};
                        father[{new_i, new_j}] = {begin_i, begin_j};
                    }
                    reachB[distance_end[d + 1][new_decay]] = true;
                }
                
                if (begin_j - 1 > 0) {
                    int new_decay = diagonal_decay - 1;
                    int new_i = begin_i;
                    int new_j = begin_j - 1;
                    if (reachA[new_i][new_j]) {
                        return get_points({new_i, new_j}, it);
                    }
                    forward_advance(new_i, new_j);
                    reachB[distance_end[d + 1][new_decay]] = false;
                    auto cell = distance_end[d + 1][new_decay];
                    if (!cell || cell.first + cell.second > new_i + new_j) {
                        distance_end[d + 1][new_decay] = {new_i, new_j};
                        father[{new_i, new_j}] = {begin_i, begin_j};
                    }
                    reachB[distance_end[d + 1][new_decay]] = true;
                }
            }
        }
    }
    vector<pair<int, string> > compute_diffs(vector<pair<int, int> >) {
        //TODO(@kira) Complete this code. Maybe tomorrow...
    }
public:
    // Returns an array of diff chunks.
    // Each chunk has a type (equal, insert or erase) and the string chunk
    vector<pair<int, string> > make_diff(string A, string B) {
        textA = A;
        textB = B;

        return compute_diffs(compute_snake());
    }
    
    Diff () {
        
    }
    
    Diff(string A, string B) {
        textA = A;
        textB = B
    }
};