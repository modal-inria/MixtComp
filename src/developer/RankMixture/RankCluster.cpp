#include "RankCluster.h"

RankCluster::RankCluster() {
  // TODO
}

RankCluster::~RankCluster() {
  // TODO
}

void RankCluster::initializeStep(double** Zik) {
  // TODO
}

void RankCluster::imputationStep(double* Pk, double** Zik) {
  // TODO
}

void RankCluster::samplingStep(double* Pk, double** Zik) {
  // TODO
}

void RankCluster::paramUpdateStep(double* Pk, double** Zik) {
  // TODO
}

void RankCluster::finalizeStep(){
  // TODO
}

double RankCluster::posteriorProbabilty(int sample_num,int Cluster_num) {
  // TODO
}

double** RankCluster::allPosteriorProbabilties() {
  // TODO
}

double RankCluster::logLikelihood() const {
  // TODO
}

int RankCluster::freeParameters() const {
  // TODO
}
