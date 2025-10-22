// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class PONG_API NeuralNetwork
{
public:

	double Frand();

	double function(FString type, double x);
	double derivative(FString type, double x);

	UPROPERTY(EditAnywhere) int n;
	UPROPERTY(EditAnywhere) double LRate = 0.1;
	UPROPERTY(EditAnywhere) TArray<int> LayerS;
	UPROPERTY(EditAnywhere) TArray<FString> LayerT;

	TArray<TArray<TArray<double>>> weight; //weights between neurons, weight[i][j][l] goes from j of layer i to neuron l of layer i+1.
	TArray<TArray<TArray<double>>> wgrad; //gradient of each weight
	TArray<TArray<double>> ngrad; // gradient of each neuron/node
	TArray<TArray<TArray<double>>> general_wgrad; //sum of the gradient of each weight of several samples
	TArray<TArray<double>> general_ngrad; //sum of the gradient of each neuron/node of several samples
	TArray<TArray<double>> value; // value of each neuron, temporal value when running
	TArray<TArray<double>> bias;//bias added to each node

	TArray<TArray<TArray<double>>> wm; // Momentum and velocity for Adam Optimizer
	TArray<TArray<double>> bm;
	TArray<TArray<TArray<double>>> wv;
	TArray<TArray<double>> bv;

	double b1 = 0.9; // hyperparameters for Adam 
	double b2 = 0.999;
	double e = 1E-8;

	void build(int N, double L, TArray<int>& s, TArray<FString>& t);

	TArray<double> run(TArray<double>& input);
	void Backprop(TArray<double>& data, int batch);
	void SingleBackprop(double data, int ind, int batch);
	void GradientDescent();
	void Adam();
	void Clean();
	void TotalClean();
	void Copy(int N, double L, TArray<int>& s, TArray<FString>& t, TArray<TArray<TArray<double>>>& w, TArray<TArray<double>>& b);
	void FullCopy(NeuralNetwork& NN);
	void GradientClipping(double clip_value);

	bool NaNError = false;

	void FixFloat(double& val);

	double MValue = ((double)1.0E50);

	double LReLUCoef = 0.01;

	NeuralNetwork();
	~NeuralNetwork();

	int it = 0;
};

