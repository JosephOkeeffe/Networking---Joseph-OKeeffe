#include <iostream>


float Factorial(float n);
float N_Choose_K(float n, float k);
float ToGetProbability(float p, float q,float n, float k);

void main()
{
	//int factorial = Factorial(6);
	//float answer = N_Choose_K(6, 0);
	float a;
	for (int i = 0; i < 500; i++)
	{
		a = ToGetProbability(0.1, 0.9, i, 2);

		if (a >= 40)
		{
			std::cout << i << "\n";
		}
	}
	

}

float Factorial(float n)
{
	float start = n;
	float result = n;

	if (n == 0)
	{
		result = 1;
	}
	else
	{
		while (n > 1)
		{
			n--;
			result *= n;
		}
		
	}
	//std::cout << "Factorial of ' " << start << " ' = " << result << "\n";
	return result;
	
}

float N_Choose_K(float n, float k)
{
	float result;

	result = (Factorial(n)) / (Factorial(k) * Factorial((n - k)));

	return result;
}

float ToGetProbability(float p, float q, float n, float k)
{
	float result;

	//std::cout << "Probability of success = p: " <<p<< "\n";
	//std::cout << "Probability of failure = q: " <<q<< "\n";
	//std::cout << "Total number of trials = n: " <<n<< "\n";
	//std::cout << "Number of successful trials = k: " <<k<< "\n";
	//std::cout << "------------------------------------------------\n";

	float part1 = N_Choose_K(n, k);
	//std::cout << "C(n,k) = C("<< n <<","<< k <<") = "  << part1 << " \n";
	float part2 = std::pow(p, k);
	//std::cout << "(p^k) = (" << p << "^" << k << ") = " << part2 << " \n";
	float part3 = std::pow(q, (n - k));
	//std::cout << "(q^(n-k)) = (" << q << "^(" << n << "-" << k << ") = " << part3 << " \n";
	result = (part1 * (part2) * part3);
	std::cout << part1 << " * " << part2 << " * " << part3 << " = " << result << "\n";
	//std::cout << "------------------------------------------------\n";
	result *= 100;

	std::cout << "Result: " << result << "% \n";
	std::cout << "------------------------------------------------\n";

	return result;
}
