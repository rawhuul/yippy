(let {x} 1)
(loop 30 {
      (
      if (&& (== (% x 3) 0) (== (% x 5) 0)) {print "FizzBuzz"}
      {(if (== (% x 3) 0) {print "Fizz"}
      {(if (== (% x 5) 0) {print "Buzz"} {print x})}
        )})
      (= {x} (+ x 1))
})