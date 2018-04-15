#include "functions.hpp"

using namespace cost_functions;

const std::vector<costFunction>& cost_functions::getAllCostFunctions(const double ** arg1, const std::size_t arg2)
{
	static std::vector<costFunction> functions
	{
		schwefelsFunction ,
		firstDeJongsFunction ,
		rosenbrockFunction ,
		rastriginFunction ,
		griewangkFunction ,
		sineEnvelopeSineWaveFunction ,
		stretchedVSineWaveFunction ,
		ackleysOneFunction ,
		ackleysTwoFunction ,
		eggHolderFunction ,
		ranaFunction ,
		pathologicalFunction ,
		michalewiczFunction ,
		masterCosineWaveFunction ,
		std::bind(shekelsFoxholesFunction, std::placeholders::_1, arg1, arg2), // arguments are bound to make the signature the same for all functions
	};

	return functions;
} // end method getAllCostFunctions

