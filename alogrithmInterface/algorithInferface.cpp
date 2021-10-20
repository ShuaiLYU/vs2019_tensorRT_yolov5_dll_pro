

#include "FDAlgorithmImplement.h"

#include <iostream>

FDAlgorithm::FDAlgorithm()
{
}



MATUREAPPROACH_API FDAlgorithm* _stdcall CreateExportObj(std::string  workspace)
{
	return new FDAlgorithmImplement(workspace);
}
