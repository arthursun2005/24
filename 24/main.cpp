//
//  main.cpp
//  24
//
//  Created by Arthur Sun on 7/3/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

FILE* finput;
FILE* foutput;

struct Solution
{
    bool possible;
    std::string method;
};

enum op_type
{
    e_addition,
    e_subtraction,
    e_multiplication,
    e_division,
    e_power
};

struct Operation
{
    op_type type;
    
    Operation(op_type type) : type(type) {}
    
    std::string getOperator() const {
        if(type == e_addition) {
            return "+";
        }
        
        if(type == e_subtraction) {
            return "-";
        }
        
        if(type == e_multiplication) {
            return "*";
        }
        
        if(type == e_division) {
            return "/";
        }
        
        if(type == e_power) {
            return "^";
        }
        
        return 0;
    }
    
    int operator () (int a, int b) const {
        if(type == e_addition) {
            return a + b;
        }
        
        if(type == e_subtraction) {
            return a - b;
        }
        
        if(type == e_multiplication) {
            return a * b;
        }
        
        if(type == e_division) {
            return a / b;
        }
        
        if(type == e_power) {
            return pow<int, int>(a, b);
        }
        
        return 0;
    }
};

struct Number
{
    int value;
    std::string representation;
    
    Number() {}
    
    Number(int value) : value(value) {
        representation = std::to_string(value);
    }
};

Operation allowed_operations[] = {
    Operation(e_addition),
    Operation(e_subtraction),
    Operation(e_multiplication),
    Operation(e_division),
    Operation(e_power)
};

const static int allowed_operations_size = sizeof(allowed_operations) / sizeof(*allowed_operations);

Solution _solve(std::vector<Number> N, const Solution& solution, int target) {
    Solution s = solution;
    
    if(N.empty()) {
        s.possible = false;
        return s;
    }
    
    size_t size = N.size();
    
    if(size == 1) {
        s.possible = N.front().value == target;
        return s;
    }
    
    for(size_t i = 0; i != size; ++i) {
        for(size_t j = i + 1; j != size; ++j) {
            for(int p = 0; p < allowed_operations_size; ++p) {
                const Operation& operation = allowed_operations[p];
                
                for(int _n = 0; _n < 2; ++_n) {
                    if(operation.type == e_division && (N[j].value == 0 || (N[i].value/N[j].value) * N[j].value != N[i].value)) {
                        std::swap(i, j);
                        continue;
                    }
                    
                    std::vector<Number> n;
                    
                    Number result;
                    
                    result.value = operation(N[i].value, N[j].value);
                    result.representation = "(" + N[i].representation + " " + operation.getOperator() + " " + N[j].representation + ")";
                    
                    size_t s1 = size - 1;
                    
                    if(i < j) {
                        for(size_t _i = 0; _i != s1; ++_i) {
                            if(_i == i) n.push_back(result);
                            else if(_i < j) n.push_back(N[_i]);
                            else n.push_back(N[_i + 1]);
                        }
                    }else{
                        for(size_t _i = 0; _i != s1; ++_i) {
                            if(_i == j) n.push_back(result);
                            else if(_i < i) n.push_back(N[_i]);
                            else n.push_back(N[_i + 1]);
                        }
                    }
                    
                    s.method = result.representation;
                    
                    Solution k = _solve(n, s, target);
                    
                    if(k.possible)
                        return k;
                    
                    std::swap(i, j);
                }
            }
        }
    }
    
    s.possible = false;
    return s;
}

inline void solve(std::vector<Number> N, Solution* solution, int target) {
    *solution = _solve(N, Solution(), target);
}

int main(int argc, const char * argv[]) {
    finput = fopen("input.txt", "r");
    foutput = fopen("output.txt", "w");
    
    Solution solution;
    
    int k;
    
    std::vector<Number> N;
    
    while(fscanf(finput, "%d\n", &k) != -1) {
        N.push_back(k);
    }
    
    solve(N, &solution, 24);
    
    if(solution.possible) {
        fprintf(foutput, "%s\n", solution.method.c_str());
    }else{
        fprintf(foutput, "Impossible\n");
    }
    
    fclose(finput);
    fclose(foutput);
    return 0;
}
