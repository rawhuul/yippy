(let {x} 1)
(loop 30 {(if (== (% x 3) 0) {print "Fizz"} {print x}) (= {x} (+ x 1))})