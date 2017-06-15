function K = kfun_sigmoid(U,V,P1,P2)
% Sigmoid ºËº¯Êý
K = tanh(P1*(U*V')+P2);