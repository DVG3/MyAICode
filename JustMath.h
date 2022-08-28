#pragma once
#include <iostream>
#include <algorithm>
#include <initializer_list>
#include <random>
#include <time.h>
#include <sstream>
namespace jmth
{
    bool setup = []() {
        srand(time(0));
        return 1;
    }();

    float GetFloatRandom()
    {
        return float(rand() % 100) / 100.0 * (rand() & 1 ? 1 : -1);
    }

    int GetIntRandom(int size)
    {
        return rand() % size * (rand() & 1 ? 1 : -1);
    }

    template <class mvar>
    struct Matrix
    {
    private:
#define _iterate(i,j,f) for (int i = 0; i < _shape[0];i++) for (int j = 0; j < _shape[1];j++) {f;}
        int _shape[2] = { 0,0 };
    public:

        /*            *******************Constructors,Destructors******************             */

        mvar** val;
        int shape(int idx) const
        {
            return _shape[idx];
        }

        Matrix(std::initializer_list<std::initializer_list<mvar>> mat)
        {
            _shape[0] = mat.size();
            _shape[1] = (*mat.begin()).size();

            val = new mvar * [_shape[0]];
            int idx = 0;
            for (auto& row : mat)
            {
                val[idx] = new mvar[_shape[1]];
                memcpy(val[idx], row.begin(), _shape[1] * sizeof(mvar));
                idx++;
            }
        }
        Matrix(int row_c, int col_c)
        {
            _shape[0] = row_c; _shape[1] = col_c;
            val = new mvar * [_shape[0]];
            for (int i = 0; i < _shape[0]; i++)
            {
                val[i] = new mvar[_shape[1]];
                for (int j = 0; j < _shape[1]; j++)
                {
                    val[i][j] = GetFloatRandom();
                }
            }
        }
        Matrix(int row_c, int col_c, float initVal)
        {
            _shape[0] = row_c; _shape[1] = col_c;
            val = new mvar * [_shape[0]];
            for (int i = 0; i < _shape[0]; i++)
            {
                val[i] = new mvar[_shape[1]];
                for (int j = 0; j < _shape[1]; j++)
                {
                    val[i][j] = initVal;
                }
            }
        }

        Matrix(const Matrix& other)
        {
            _shape[0] = other._shape[0]; _shape[1] = other._shape[1];
            val = new mvar * [other._shape[0]];
            for (int i = 0; i < other._shape[0]; i++)
            {
                val[i] = new mvar[other._shape[1]];
                memcpy(val[i], other.val[i], sizeof(mvar) * other._shape[1]);
            }
        }


        ~Matrix()
        {
            for (int i = 0; i < _shape[0]; i++)
            {
                delete[] val[i];
            }
            delete[] val;
        }


        /*            *******************Operators******************             */

#define _CalculateOperator(x) Matrix operator x (const Matrix & other)\
    {\
        if (_shape[0] != other._shape[0] || _shape[1] != other._shape[1]) { \
        std:: stringstream ss; ss << "_shape [" << _shape[0] << "," << _shape[1] << "] cannot match with [" \
        << other._shape[0] << "," << other._shape[1] << "]";\
        std::cerr << ss.str();\
        throw;\
        }\
        Matrix res(_shape[0],_shape[1],0);\
        for (int i = 0; i < _shape[0];i++)\
        {\
            for (int j = 0; j < _shape[1]; j++)\
            {\
                res[i][j] = val[i][j] x other[i][j];\
            }\
        }\
        return res;\
    }\

#define _SelfCalculateOperator(x) void operator x (const Matrix & other)\
    {\
        if (_shape[0] != other._shape[0] || _shape[1] != other._shape[1]) { \
        std:: stringstream ss; ss << "_shape [" << _shape[0] << "," << _shape[1] << "] cannot match with [" \
        << other._shape[0] << "," << other._shape[1] << "]";\
        std::cerr << ss.str();\
        throw;\
        }\
        for (int i = 0; i < _shape[0];i++)\
        {\
            for (int j = 0; j < _shape[1]; j++)\
            {\
                val[i][j] x other[i][j];\
            }\
        }\
    }\

#define _CalculateOperatorSingle(x) Matrix operator x (const float &num)\
    {\
        Matrix res(_shape[0],_shape[1],0);\
        for (int i = 0; i < _shape[0];i++)\
        {\
            for (int j = 0; j < _shape[1]; j++)\
            {\
                res[i][j] = val[i][j] x num;\
            }\
        }\
        return res;\
    }

#define _SelfCalculateOperatorSingle(x) void operator x (const float &num)\
    {\
        for (int i = 0; i < _shape[0];i++)\
        {\
            for (int j = 0; j < _shape[1]; j++)\
            {\
                val[i][j] x num;\
            }\
        }\
    }

        _CalculateOperator(+); _CalculateOperator(-); _CalculateOperator(*); _CalculateOperator(/ );
        _SelfCalculateOperator(+= ); _SelfCalculateOperator(-= ); _SelfCalculateOperator(*= ); _SelfCalculateOperator(/= );
        _CalculateOperatorSingle(*); _SelfCalculateOperatorSingle(*= );

        mvar* operator[] (int idx)
        {
            return val[idx];
        }

        mvar* operator[] (int idx) const
        {
            return val[idx];
        }

        Matrix Dot(const Matrix& other)
        {
            if (_shape[1] != other.shape(0)) {

                std::stringstream ss; ss << "_shape [" << _shape[0] << "," << _shape[1] << "] cannot match with ["
                    << other._shape[0] << "," << other._shape[1] << "]";
                std::cerr << ss.str();
                throw;
            }
            Matrix res = { _shape[0],other._shape[1],0 };
            for (int i = 0; i < _shape[0]; i++)
            {
                for (int j = 0; j < other._shape[1]; j++)
                {
                    for (int k = 0; k < _shape[1]; k++)
                    {
                        res[i][j] += val[i][k] * other[k][j];
                    }
                }
            }
            return res;
        }


        /*            *******************Functions******************             */

        template <typename func>
        void map(func f) {
            _iterate(i, j, {
                val[i][j] = f(val[i][j]);
                })
        }

        template <typename func>
        Matrix Map(func f)
        {
            Matrix res = { _shape[0],_shape[1],0 };
            _iterate(i, j, {
                res[i][j] = f(val[i][j]);
                });
            return res;
        }

        Matrix Transpose()
        {
            Matrix res = { _shape[1],_shape[0],0 };
            for (int i = 0; i < _shape[0]; i++)
            {
                for (int j = 0; j < _shape[1]; j++)
                {
                    res[j][i] = val[i][j];
                }
            }
            return res;
        }


    };

    template<class T>
    std::ostream& operator<<(std::ostream& out, const Matrix<T>& a)
    {
        for (int i = 0; i < a.shape(0); i++)
        {
            out << "[";
            for (int j = 0; j < a.shape(1); j++)
            {
                out << a[i][j] << " ";
            }
            out << "\b]\n";
        }
        return out;
    }


}


