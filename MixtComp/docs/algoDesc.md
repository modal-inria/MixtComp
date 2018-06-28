This file contains a summary description of the algorithms, and outline

The easiest way to understand how to program MixtComp might be to add a new model from scratch, and

The learning case is considered, because once it has been understood, the prediction algorithm becomes easy.

This is a general overview of the algorithm. If one method is called from another, it is shown with one additional level of indentation. Reading order is calling order.

The SemStrategy and GibbsStrategy are coded in the [Strategy](./MixtComp/src/lib/Strategy) folder. The core algorithm is contained here.

# R interface

See [mixtCompCluster.cpp](./RMixtComp/src/mixtCompCluster.cpp) for the entry point to the learning algorithm, and [mixtCompPredict.cpp](./RMixtComp/src/mixtCompPredict.cpp) for the entry point to the predict algorithm.

# SemStrategy run

Source [here](./MixtComp/src/lib/Strategy/mixt_SEMStrategy.cpp).

The main difference between an SEM run and a Gibbs run is that estimations take place. Those estimation must be tracked, and they can also wrong. The SEMStrategy is similar, except that no estimation take place, and the parameters are fixed all along.

Things can go wrong at various points in the algorithm, for example there can be errors in initialization, or degeneracy in the general execution. That is why there is a global loop `for (Index n = 0; n < param_.nSemTry_; ++n)` that is run `param_.nSemTry_` times at maximum. If an iteration is completed, the loop is exited. If no iteration can be completed, the `std::string` error from the last iteration is returned.

# GibbsStrategy run

The GibbsStrategy can go wrong, for example if an individual has a 0 probability in every class (there are `nSemTry_`). The difference with `SemStrategy` is that, once initialization has been performed, there can be no error, because parameters have already been estimated, therefor degeneracy is not a problem. This is why `GibbsStrategy::runGibbs` returns `void`, not a `std::string`.

There is a try / catch exception handling, which main purpose is to avoid crashes. In a normal run, it is impossible for $t_{ik}$ values to be `nan`. However, errors in implementations in the model can lead to this. To avoid this kind of problems, an exception can be thrown from `MixtureComposer::eStepCompleted`. The program will then end normally, urging the user to contact the maintainer.
