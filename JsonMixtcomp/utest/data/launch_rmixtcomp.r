source("cylande/retfor/livrable_2/tools/tools_mixtcomp.r")
system.time({
  mixtcomp_learning(nbClass=5, dataPath, descriptorPath,n_iter=20,n_burnin_iter=20,gibbs_n_iter=20,gibbs_n_burnin_iter=20,n_parallel_mixtcomp=1)
})