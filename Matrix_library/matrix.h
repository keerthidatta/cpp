#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>
#include <array>
#include <algorithm>

template <typename T>
class matrix
{
    public:
        matrix(int rows, int cols): container_(rows*cols, 0.0), rows_(rows), columns_(cols){}
        matrix():container_(0), rows_(0), columns_(0){}
        T operator ()(int row, int col)
        {
            int position = col + columns_ * row;
            try{
                if(row * col > (rows_ -1)*(columns_-1))
                    throw std::out_of_range("Index out of range : Matrix is zero indexed : (0, 0) is the first row first column of the matrix");
            }
            catch(std::out_of_range exception){
                std::cout << exception.what() << std::endl;
                return -1;
            }
            return container_.at(position);
        }
        void operator =(const std::vector<T>&& val)
        {
            try{
                if(val.size() == rows_*columns_)
                    container_ = val;
                else
                    throw std::invalid_argument("Invalid dimensions: Matrix values not assigned");
            }
            catch(std::invalid_argument exception)
            {
                std::cerr << exception.what() << std::endl;
            }
        } 
        matrix operator +(const matrix &mat)
        {
            matrix add(mat.rows_, mat.columns_);
            try{
                if (rows_ == mat.rows_ && columns_ == mat.columns_)
                {
                    for(int i=0; i<rows_*columns_; i++)
                    {
                        add.container_.at(i) = container_.at(i) + mat.container_.at(i); 
                    }
                }
                else
                    throw std::runtime_error("Invalid operation: Cannot add matrices of different dimensions: ");
            }
            catch(std::runtime_error exception)
            {
                matrix<T> err;
                std::cerr << exception.what() << "("<< rows_ << "," << columns_ << ") and (" << mat.rows_ << "," << mat.columns_<< ")" << std::endl;
                return err;
            }
            return add;
        }
        matrix operator -(const matrix &mat)
        {
            matrix sub(mat.rows_, mat.columns_);
            try
            {
                if (rows_ == mat.rows_ && columns_ == mat.columns_)
                {
                    for(int i=0; i<rows_*columns_; i++)
                    {
                        sub.container_.at(i) = container_.at(i) - mat.container_.at(i); 
                    }
                }
                else
                    throw std::runtime_error("Invalid operation: Cannot subtract matrices of different dimensions: ");
            }
            catch(std::runtime_error exception)
            {
                matrix<T> err;
                std::cerr << exception.what() << "("<< rows_ << "," << columns_ << ") and (" << mat.rows_ << "," << mat.columns_<< ")" << std::endl;
                return err;
            }
            return sub;
        }
        friend matrix operator *(const T scalar_const, const matrix& mat) 
        {
            matrix<T> scalar(mat.rows_, mat.columns_);
            for(auto pos=0; pos < mat.rows_*mat.columns_; pos++)
            {
                scalar.container_.at(pos) = scalar_const * mat.container_.at(pos);
            }
            return scalar;
        }

        matrix operator *(const T scalar_const)
        {
            matrix<T> scalar(rows_, columns_);
            for(auto pos=0; pos < rows_*columns_; pos++)
            {
                scalar.container_.at(pos) = scalar_const * container_.at(pos);
            }
            return scalar;
        }

        matrix operator *(const matrix& mat)
        {
            try
            {
                if(rows_ != mat.rows_ && rows_ != mat.columns_ &&
                columns_ != mat.columns_ && columns_ != mat.rows_)
                {
                    throw std::runtime_error("Matrix Multiplication not possible for matrices of shape : ");
                }
                else if (columns_ != mat.rows_)
                {
                    throw std::runtime_error("Use Reshape function to match inner dimensions: Matrix Multiplication not possible for matrices of shape : ");
                }
            }
            catch(const std::runtime_error& exception)
            {
                matrix<T> err;
                std::cerr << exception.what() << "("<< rows_ << "," << columns_ << ") and (" << mat.rows_ << "," << mat.columns_<< ")" << std::endl;
                return err;
            }
            
            matrix<T> mul(rows_, mat.columns_);
            std::vector<T> multiplied_container;
            int size = columns_;

            for(int row=0; row<rows_; row++)
            {
                for(int col=0; col < mat.columns_; col++)
                {
                    T val = 0;
                    for(int elem = 0; elem<size; elem++)
                    {
                        int position_matrix_a = elem + columns_ * row;
                        int position_matrix_b = col + mat.columns_ * elem;
                        val += container_.at(position_matrix_a) * mat.container_.at(position_matrix_b);
                    }
                    multiplied_container.push_back(val);
                }
            }
            mul.container_ = multiplied_container;
            return mul;
        }
        void reshape(int rows, int columns);
        std::array<int, 2> getShape();
        matrix Transpose();
        std::vector<T> getContainer(){return container_;}
        matrix elementwise_multiply(const matrix& mat);
        bool squareMatrix(const matrix& mat);
        void printContainer();

    private:
        int rows_;
        int columns_;
        std::vector<T> container_;
};

template<typename T> std::array<int, 2> matrix<T>::getShape()
{
    std::array<int, 2> shape;
    shape[0] = rows_;
    shape[1] = columns_;
    return shape;
}
template<typename T> void matrix<T>::printContainer()
{
    std::cout << std::endl;
    for(auto i=0; i<rows_; i++)
    {
        for(auto j=0; j<columns_; j++)
        {
            int position = j + columns_ * i;
            std::cout << container_.at(position) << " ";
        }
        std::cout << std::endl;
    }
}
template<typename T> bool matrix<T>::squareMatrix(const matrix& mat)
{
    return (mat.rows_ == mat.columns_) ? true : false;
}
template<typename T> void matrix<T>::reshape(int rows, int columns)
{
    try
    {
        if(rows*columns == rows_ * columns_)
        {
            rows_ = rows;
            columns_ = columns;
        }
        else
            throw std::invalid_argument("Operation failed: Matrix dimension reshape not possible : dimension mismatch");
    }
    catch(const std::invalid_argument& exception)
    {
        std::cerr << exception.what() << "("<< rows_ << "," << columns_ << ") and (" << rows << "," << columns << ")" << std::endl;
    }
}
template<typename T> matrix<T> matrix<T>::Transpose()
{
    std::vector<T> transposed;
    for(int col=0; col<columns_; col++)
    {
        for(int row = 0; row < rows_; row++)
        {
            int position = col + columns_ * row;
            transposed.push_back(container_.at(position));
        } 
    }
    matrix transposed_(columns_, rows_);
    transposed_.container_ = transposed;
    return transposed_;
}
template<typename T> matrix<T> matrix<T>::elementwise_multiply(const matrix& mat) 
{
    matrix<T> element_mul(rows_, columns_);
    try
    {
        if(mat.rows_ == rows_ && mat.columns_ == columns_)
        {
            for(auto pos=0; pos < mat.rows_*mat.columns_; pos++)
            {
                element_mul.container_.at(pos) = container_.at(pos)  * mat.container_.at(pos);
            }
        }
        else
            throw std::runtime_error("Invalid Arguments: Cannot perform element-wise multiplication: ");
    }
    catch(std::runtime_error exception)
    {
        matrix<T> err;
        std::cerr << exception.what() << "("<< rows_ << "," << columns_ << ") and (" << mat.rows_ << "," << mat.columns_<< ")" << std::endl;
        return err;
    }
    return element_mul;
}

#endif