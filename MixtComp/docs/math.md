# Mathematical background of some elements of MixtComp

## Initialization of latent and Unobserved variables in initializeLatent()

The joint distribution of the data $`x`$ and $`w`$ can be used to initialize a latent or unobserved variables (say $`w_i^j`$) of the $`i`$-th individual for the  $`j`$-th variable using current estimated parameters. This distribution is:

```math
p(w_i^j, x_i^j) = \sum_{k} p(w_i^j, x_i^j | z = k) * p(z = k)
```

Sampling according to this law can be done by:

1. Sampling the class $`k`$ according to a multinomial with the current proportion $`(p(z = j))_{j=1,...,K}`$ as probabilities
2. Sampling $`w_i^j`$ according to $`p(w_i^j, x_i^j | z = k)`$
  
The first step is done by calling `sampleZProportion()` and the second by calling `sampleUnobservedAndLatent()`.
`sampleZProportion()` first assigns proportion to each row of `tik_` and then calls `sampleZ()`.
