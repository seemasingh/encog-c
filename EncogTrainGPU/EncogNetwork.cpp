#include "encog.h"

void EncogNetworkFree(ENCOG_NETWORK *network)
{
	free(network->contextTargetOffset);
	free(network->contextTargetSize);
	free(network->layerCounts);
	free(network->layerFeedCounts);
	free(network->layerContextCount);
	free(network->layerIndex);
	free(network->output);
	free(network->weightIndex);
	free(network->weights);
	free(network->biasActivation);
	free(network->activationFunction);
	free(network);
}

void EncogNetworkCompute(ENCOG_NETWORK *network, double *inputData, double *outputData )
{
	
}
