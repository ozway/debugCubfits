### Function to run MH draw for phi given observed (with log-Normal error) phi.Obs,
### count y, number of codons n, coefficients b, and measurement error
### log-variance sigmaWsq.
###
### Performs vectorized draw.
### Assumes phi.Curr, phi.Obs, y, & n are vectors of length # of genes;
### b is 2-dim vector; sigmaWsq is scalar.
###
### Returns list with elements:
###   * phi.New  :   new value for phi resulting from MH draw
###   * accept:   boolean vector indicating if each proposal was accepted

my.drawPhiConditionalAll <- function(phi.Curr, phi.Obs, y, n, b,
    p.Curr, reu13.df = NULL){
  ### Propose new phi.
  prop <- .cubfitsEnv$my.proposePhiAll(phi.Curr)

  ### Calculate acceptance prob.
  lpCurr <- .cubfitsEnv$my.logPosteriorAll(
              phi.Curr, phi.Obs, y, n, b, p.Curr, reu13.df = reu13.df)
  lpProp <- .cubfitsEnv$my.logPosteriorAll(
              prop$phi.Prop, phi.Obs, y, n, b, p.Curr, reu13.df = reu13.df)
	
# tmp.phi <- rep(prop$phi.Prop, n[[4]])
# xm <- matrix(cbind(1, tmp.phi * reu13.df[[4]]$Pos), ncol = 2)
# baamat <- matrix(b[[4]], nrow = 2, byrow = TRUE)
# lp.vec <- my.inverse.mlogit(xm %*% baamat, log = TRUE)

##One of these contains NaN in the NSE model
##I suspect lpProp, proposed Phi values

#write(matrix(prop$phi.Prop, nrow=1), "ne-phiprop.csv", ncolumns=length(prop$phi.Prop), append=TRUE);
#write(matrix(phi.Curr, nrow=1), "ne-phicurr.csv", ncolumns=length(phi.Curr), append=TRUE);

# if (TRUE%in%is.nan(lpProp)){	print("ALERT ALERT ALERT lpProp has NaN!"); browser();
# while(TRUE%in%is.nan(lpProp)){	print("ALERT ALERT ALERT lpProp has NaN!"); browser(); }
# }

#if (TRUE%in%is.nan(lpCurr)){	print("ALERT ALERT ALERT lpCurr has NaN!"); browser();	}
#if (TRUE%in%is.nan(prop$lir)){	print("ALERT ALERT ALERT prop$lir has NaN!"); browser();	}


  logAcceptProb <- lpProp - lpCurr - prop$lir

#browser();

  ### Run MH acceptance rule.
  u <- runif(length(phi.Curr))
  accept <- u < exp(logAcceptProb)
  phi.New <- phi.Curr
  phi.New[accept] <- prop$phi.Prop[accept]

  ### Extra update and trace.
  my.update.acceptance("phi", accept)
  my.update.adaptive("phi", accept)
    
  ### Return.
  ret <- phi.New
  ret
} # End of my.drawPhiConditionalAll().

