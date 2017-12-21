cardio
======

This tests implementation the medical diagnostic algorithm that was presented in paper S. Carpov, T.-H. Nguyen, R. Sirdey, G. Constantino and F. Martinelli, "Practical privacy-preserving medical diagnosis using homomorphic encryption", Proceedings of the 9th IEEE Conference on Cloud Computing, pp. 593-599, 2016 ([DOI](http://dx.doi.org/10.1109/CLOUD.2016.0084)).

This test is intesting to look at (and possibly clone) as it mixes (non-constant at compile time) clear and encrypted inputs.

The clear inputs are in fact symmetrically encrypted data (with the Kreyvium stream cipher, see the kreyvium directory and the README therein) and the encrypted inputs define the keystream for transchiphering them to FHE.

So it's a more involved test - to be looked at after you are familiar with more basic CINGULATA programming stuff.

