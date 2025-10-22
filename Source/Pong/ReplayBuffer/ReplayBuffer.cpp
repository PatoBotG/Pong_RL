
#include "Pong/ReplayBuffer/ReplayBuffer.h"

void FReplayBuffer::Build(int sz) {
	max_size = sz;
	current_size = 0;
	index = -1;
	Buffer.SetNum(max_size);
}

void FReplayBuffer::Add(FDataPiece data) {
	if (current_size < max_size) {
		Buffer[index + 1] = data;
		index++;
		current_size++;
	}
	else if (index != max_size - 1) {
		Buffer[index + 1] = data;
		index++;
	}
	else {
		index = 0;
		Buffer[index] = data;
	}
}

TArray<FDataPiece> FReplayBuffer::RandomSample(int batchlen) {
	TArray<FDataPiece> sample;
	sample.SetNum(batchlen);
	int i = 0;
	TSet<int> taken;

	while (i < batchlen) {
		int j = FMath::RandRange(0, current_size - 1);
		if (!taken.Contains(j)) {
			sample[i] = Buffer[j];
			taken.Add(j);
			i++;
		}
	}

	return sample;
}

