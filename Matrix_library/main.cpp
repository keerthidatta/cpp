#include "matrix.h"

int main()
{
    //Initialize matrix
    matrix<float> c(3, 2), d(3, 2);
    c = {1.0, 2.0, 3.0, 4.0, 5.0, 4.0};
    d = {3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
    d.printContainer();
    
    //Addition of two matrices
    matrix<float> x = c + d;
    x.printContainer();

    //Subtraction of two matrices
    x = c - d;
    x.printContainer();

    //check value of position in a matrix : starts with (0, 0) 
    std::cout << "value at x(2, 1) " << x(2, 1) << std::endl;
    
    matrix<float> y = x.Transpose();
    y.printContainer();


    //Reshape example
    matrix<int> test_reshape(2, 6);
    test_reshape = {1, 9, 6, 7, 4, 8, 90, 10, 13, 49, 12, 16};
    test_reshape.printContainer();
    test_reshape.reshape(4, 3);
    test_reshape.printContainer();


    matrix<int> test_transpose = test_reshape.Transpose();
    test_transpose.printContainer();
    std::cout << test_transpose.getShape()[0] << ", " << test_transpose.getShape()[1] << std::endl;

    //Empty constructor;
    matrix<int> m;

    //Example of matrix multiplication
    matrix<float> matmul_a(2, 3), matmul_b(2, 3);
    matmul_a = {1, 2, 3, 4, 5, 6};
    matmul_b = {1, 2, 3, 4, 3.2, 7.5};
    matmul_a.printContainer();
    matmul_b.printContainer();

    matrix<float> matmul_c = matmul_a * matmul_b;

    //reshape to do matmul between matmul_a, matmul_b
    matmul_b.reshape(3,2);
    matmul_c = matmul_a * matmul_b;
    matmul_c.printContainer(); 

    //Element wise multiplication and scalar multiplication
    matrix<float> elem_mul = 2 * matmul_c.elementwise_multiply(matmul_c) * 2;
    elem_mul.printContainer();
    //get shape of the matrix
    std::cout << elem_mul.getShape()[0] << ", " << elem_mul.getShape()[1] << std::endl;

    //vectors

    matrix<float> a(1, 4), b(1, 4);
    a = {1.0, 2.0, 3.0, 4.0};
    b = {3.0, 4.0, 5.0, 6.0};
    
    //Addition of two vectors
    matrix<float> z = a + b;
    z.printContainer();
    z = a - b;
    z.printContainer();

    //Multiplication of a vector and matrix
    matrix<float> f(4, 4);
    f = {1,2, 3,4,5,5,6,6,7,7,8,8,9,10, 11, 12} ;
    z = a * f;
    z.printContainer();
}
