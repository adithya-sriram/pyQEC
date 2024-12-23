# pyClifford

This is an implementation of Clifford circuit simulations as described in https://arxiv.org/abs/quant-ph/0406196. Once built, one can access the functions with python bindings. This also contains code to run measurement simulations on a honeycomb, as done in the following: https://arxiv.org/abs/2107.02194, https://arxiv.org/abs/2207.02877, https://arxiv.org/abs/2207.07096. 

This code was written early on in my graduate career and as such is a suboptimal implementation. It does not make full use of the GF(2) operations that make manipulating Pauli strings highly efficient. As a result, naive usage of this library is not recommended.

This library will require pybind in a submodule to run.
