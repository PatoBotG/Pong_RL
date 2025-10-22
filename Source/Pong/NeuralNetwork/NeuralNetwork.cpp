
#include "Pong/NeuralNetwork/NeuralNetwork.h"

NeuralNetwork::NeuralNetwork()
{
}

NeuralNetwork::~NeuralNetwork()
{
}

double NeuralNetwork::Frand() {
	return 2.0 * ((double)(rand()) / (double)(RAND_MAX)) - 1.0;
}

void NeuralNetwork::FixFloat(double& val) {
	val = FMath::Max(-MValue, FMath::Min(MValue, val));
	return;
}

double NeuralNetwork::function(FString type, double x) {
	double ans = x;
	if (type == "None") {

		ans = x;
	}
	else if (type == "Cube") {
		ans = x * x * x;
	}
	else if (type == "Sigmoid") {
		if (x < -50.0f) {
			ans = 0.0f;
		}
		else if (x > 50.0f) {
			ans = 1.0f;
		}
		else {
			ans = 1 / (1 + FMath::Exp(-x));
		}
	}
	else if (type == "LeakyReLU") {
		ans = FMath::Max(0.0f, x) - LReLUCoef * FMath::Min(0.0f, x);
	}
	FixFloat(ans);
	return ans;
}

double NeuralNetwork::derivative(FString type, double x) {
	double ans = 1.0f;
	if (type == "None") {
		ans = 1.0f;
	}
	else if (type == "Cube") {
		ans = 3 * x * x;
	}
	else if (type == "Sigmoid") {
		ans = (function(type, x) * (1 - function(type, x)));
	}
	else if (type == "LeakyReLU") {
		if (x >= 0.0f) {
			ans = 1.0f;
		}
		else {
			ans = -LReLUCoef;
		}
	}
	return ans;
}

void NeuralNetwork::build(int N, double L, TArray<int>& s, TArray<FString>& t) {

	//UE_LOG(LogTemp, Warning, TEXT("Test 1 passed"));
	n = N;
	LRate = L;
	LayerS = s;
	LayerT = t;

	//UE_LOG(LogTemp, Warning, TEXT("Test 2 passed"));

	weight.SetNum(N - 1);
	wgrad.SetNum(N - 1);
	ngrad.SetNum(N);
	general_wgrad.SetNum(N - 1);
	general_ngrad.SetNum(N);
	value.SetNum(n);
	bias.SetNum(n);

	wm.SetNum(N - 1);
	bm.SetNum(n);
	wv.SetNum(N - 1);
	bv.SetNum(n);
	//UE_LOG(LogTemp, Warning, TEXT("Test 3 passed"));


	for (int i = 0; i < n - 1; i++) {
		weight[i].SetNum(LayerS[i]);
		wgrad[i].SetNum(LayerS[i]);
		general_wgrad[i].SetNum(LayerS[i]);
		wm[i].SetNum(LayerS[i]);
		wv[i].SetNum(LayerS[i]);
		for (int j = 0; j < LayerS[i]; j++) {
			weight[i][j].SetNum(LayerS[i + 1]);
			wgrad[i][j].SetNum(LayerS[i + 1]);
			general_wgrad[i][j].SetNum(LayerS[i + 1]);
			wm[i][j].SetNum(LayerS[i + 1]);
			wv[i][j].SetNum(LayerS[i + 1]);
			for (int l = 0; l < LayerS[i + 1]; l++) {
				weight[i][j][l] = Frand();
				wm[i][j][l] = 0;
				wv[i][j][l] = 0;
			}
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("Test 4 passed"));


	for (int i = 0; i < n; i++) {
		ngrad[i].SetNum(LayerS[i]);
		value[i].SetNum(LayerS[i]);
		general_ngrad[i].SetNum(LayerS[i]);
		bias[i].SetNum(LayerS[i]);
		bm[i].SetNum(LayerS[i]);
		bv[i].SetNum(LayerS[i]);
		for (int j = 0; j < LayerS[i]; j++) {
			bias[i][j] = Frand();
			bm[i][j] = 0;
			bv[i][j] = 0;
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("Test 5 passed"));


	return;
}

TArray<double> NeuralNetwork::run(TArray<double>& input) {
	Clean();
	if (input.Num() != LayerS[0]) {
		return {};
	}

	for (int i = 0; i < LayerS[0]; i++) {
		value[0][i] = input[i];
		if (FMath::IsNaN(value[0][i]))
		{
			UE_LOG(LogTemp, Warning, TEXT("Value (%d, %d) is NaN"), 0, i);
			NaNError = true;
			break;
		}
		FixFloat(value[0][i]);
	}

	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < LayerS[i]; j++) {
			for (int l = 0; l < LayerS[i + 1]; l++) {
				value[i + 1][l] += value[i][j] * weight[i][j][l];
				//FixFloat(value[i+1][l]);
				if (FMath::IsNaN(value[i + 1][l]))
				{
					UE_LOG(LogTemp, Warning, TEXT("Value (%d, %d) is NaN, j=%d"), i + 1, l, j);
					UE_LOG(LogTemp, Warning, TEXT("Obtained from v = %f and w = %f"), value[i][j], weight[i][j][l]);

					NaNError = true;
				}
				if (FMath::IsNaN(weight[i][j][l]))
				{
					UE_LOG(LogTemp, Warning, TEXT("Weight (%d, %d, %d) is NaN"), i, j, l);
					NaNError = true;
				}
				if (NaNError) {
					break;
				}
			}
			if (NaNError) {
				break;
			}
		}
		if (NaNError) {
			break;
		}

		for (int l = 0; l < LayerS[i + 1]; l++) {
			value[i + 1][l] += bias[i + 1][l];
			FixFloat(value[i + 1][l]);
			value[i + 1][l] = function(LayerT[i + 1], value[i + 1][l]);
			FixFloat(value[i + 1][l]);
		}
	}

	return value[n - 1];
}

void NeuralNetwork::Backprop(TArray<double>& data, int batch) {
	if (NaNError) {
		return;
	}

	//Clean();

	for (int i = 0; i < LayerS[n - 1]; i++) {
		ngrad[n - 1][i] = (2.0f * (value[n - 1][i] - data[i])) / (double(batch));
		ngrad[n - 1][i] *= derivative(LayerT[n - 1], value[n - 1][i]);
		//FixFloat(ngrad[n - 1][i]);

	}

	for (int i = n - 2; i >= 0; i--) {
		for (int l = 0; l < LayerS[i + 1]; l++) {
			ngrad[i + 1][l] *= derivative(LayerT[i + 1], value[i + 1][l]);
			//FixFloat(ngrad[i + 1][l]);
			general_ngrad[i + 1][l] += ngrad[i + 1][l];
			for (int j = 0; j < LayerS[i]; j++) {
				wgrad[i][j][l] = ngrad[i + 1][l] * value[i][j];
				//FixFloat(wgrad[i][j][l]);
				general_wgrad[i][j][l] += wgrad[i][j][l];

				ngrad[i][j] += ngrad[i + 1][l] * weight[i][j][l];
			}
			//ngrad[i + 1][l] /= (double)batch;
		}
	}
}

void NeuralNetwork::SingleBackprop(double data, int ind, int batch) {
	TArray<double> expected = value[n - 1];
	expected[ind] = data;
	Backprop(expected, batch);
}

void NeuralNetwork::GradientDescent() {
	if (NaNError) {
		return;
	}

	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < LayerS[i]; j++) {
			for (int l = 0; l < LayerS[i + 1]; l++) {
				FixFloat(general_wgrad[i][j][l]);
				weight[i][j][l] += (-LRate) * (general_wgrad[i][j][l]);
				FixFloat(weight[i][j][l]);

			}
		}
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < LayerS[i]; j++) {
			FixFloat(general_ngrad[i][j]);

			bias[i][j] += (-LRate) * (general_ngrad[i][j]);
			FixFloat(bias[i][j]);

		}
	}
}

void NeuralNetwork::Adam() {
	if (NaNError) {
		return;
	}

	it++;
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < LayerS[i]; j++) {
			for (int l = 0; l < LayerS[i + 1]; l++) {

				//FixFloat(general_wgrad[i][j][l]);

				wm[i][j][l] = b1 * wm[i][j][l] + (1 - b1) * general_wgrad[i][j][l];
				wv[i][j][l] = b2 * wv[i][j][l] + (1 - b2) * general_wgrad[i][j][l] * general_wgrad[i][j][l];

				double mh = wm[i][j][l] / (1 - FMath::Pow(b1, it));
				double vh = wv[i][j][l] / (1 - FMath::Pow(b2, it));

				weight[i][j][l] = weight[i][j][l] - LRate * (mh / (FMath::Sqrt(vh + e)));

				FixFloat(weight[i][j][l]);
			}
		}
	}
	double p1 = (1 - FMath::Pow(b1, it));
	double p2 = (1 - FMath::Pow(b2, it));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < LayerS[i]; j++) {

			bm[i][j] = b1 * bm[i][j] + (1 - b1) * general_ngrad[i][j];
			bv[i][j] = b2 * bv[i][j] + (1 - b2) * general_ngrad[i][j] * general_ngrad[i][j];

			double mh = bm[i][j] / p1;
			double vh = bv[i][j] / p2;

			bias[i][j] = bias[i][j] - LRate * (mh / (FMath::Sqrt(vh + e)));
			//FixFloat(bias[i][j]);

		}
	}
}

void NeuralNetwork::Clean() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < LayerS[i]; j++) {
			value[i][j] = 0.0f;
			ngrad[i][j] = 0.0f;
			//general_ngrad[i][j] = 0.0f;
		}
	}

	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < LayerS[i]; j++) {
			for (int l = 0; l < LayerS[i + 1]; l++) {
				wgrad[i][j][l] = 0.0f;
				//general_wgrad[i][j][l] = 0.0f;
			}
		}
	}
}

void NeuralNetwork::TotalClean() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < LayerS[i]; j++) {
			value[i][j] = 0.0f;
			ngrad[i][j] = 0.0f;
			general_ngrad[i][j] = 0.0f;
		}
	}

	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < LayerS[i]; j++) {
			for (int l = 0; l < LayerS[i + 1]; l++) {
				wgrad[i][j][l] = 0.0f;
				general_wgrad[i][j][l] = 0.0f;
			}
		}
	}
}

void NeuralNetwork::Copy(int N, double L, TArray<int>& s, TArray<FString>& t, TArray<TArray<TArray<double>>>& w, TArray<TArray<double>>& b) {
	n = N;
	LRate = L;
	LayerS = s;
	LayerT = t;

	//UE_LOG(LogTemp, Warning, TEXT("Test 2 passed"));

	weight = w;
	general_wgrad.SetNum(N - 1);
	wgrad.SetNum(N - 1);
	general_ngrad.SetNum(N - 1);
	ngrad.SetNum(N);
	value.SetNum(n);
	bias = b;

	wm.SetNum(N - 1);
	bm.SetNum(n);
	wv.SetNum(N - 1);
	bv.SetNum(n);
	//UE_LOG(LogTemp, Warning, TEXT("Test 3 passed"));


	for (int i = 0; i < n - 1; i++) {
		general_wgrad[i].SetNum(LayerS[i]);
		wgrad[i].SetNum(LayerS[i]);
		wm[i].SetNum(LayerS[i]);
		wv[i].SetNum(LayerS[i]);
		for (int j = 0; j < LayerS[i]; j++) {
			general_wgrad[i][j].SetNum(LayerS[i + 1]);
			wgrad[i][j].SetNum(LayerS[i + 1]);
			wm[i].SetNum(LayerS[i + 1]);
			wv[i].SetNum(LayerS[i + 1]);
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("Test 4 passed"));


	for (int i = 0; i < n; i++) {
		ngrad[i].SetNum(LayerS[i]);
		value[i].SetNum(LayerS[i]);
		general_ngrad[i].SetNum(LayerS[i]);
		bm.SetNum(LayerS[i]);
		bv.SetNum(LayerS[i]);
	}
}

void NeuralNetwork::FullCopy(NeuralNetwork& NN) {
	n = NN.n;
	LRate = NN.LRate;
	LayerS = NN.LayerS;
	LayerT = NN.LayerT;
	weight = NN.weight;
	bias = NN.bias;
	general_wgrad = NN.general_wgrad;
	general_ngrad = NN.general_ngrad;
	wgrad = NN.wgrad;
	ngrad = NN.ngrad;
	value = NN.value;
	bm = NN.bm;
	bv = NN.bv;
	wm = NN.wm;
	wv = NN.wv;
	b1 = NN.b1;
	b2 = NN.b2;
	e = NN.e;
	it = NN.it;
}

void  NeuralNetwork::GradientClipping(double clip_value) {
	double norm = 0.000000001;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < LayerS[i]; j++) {
			double h = general_ngrad[i][j];
			norm += h * h;
		}
	}
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < LayerS[i]; j++) {
			for (int l = 0; l < LayerS[i + 1]; l++) {
				double h = general_wgrad[i][j][l];
				norm += h * h;
			}
		}
	}

	if (norm <= clip_value) {
		return;
	}

	double scale = clip_value / norm;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < LayerS[i]; j++) {
			general_ngrad[i][j] *= scale;
		}
	}
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < LayerS[i]; j++) {
			for (int l = 0; l < LayerS[i + 1]; l++) {
				general_wgrad[i][j][l]*=scale;
			}
		}
	}
}

