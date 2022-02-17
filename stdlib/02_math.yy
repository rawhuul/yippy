;; Mathematical Constants
(let {pi} 3.141592)
(let {e} 2.718281)
(let {tau} 6.283185)

;; Aliases to common mathematical functions.
(let {add} +)
(let {sub} -)
(let {mult} *)
(let {prod} *)
(let {div} /)
(let {mod} %)
(let {remain} %)

;; Aliases to logical functions.
(let {and} &&)
(let {or} ||)

;; Power function.
(let {pow} **)

;; Log function
(fn {log b n} {(if (> n 1) {(+ 1 (log b (/ n b)))} {(- n 1)})})
(fn {log2 n} {log 2 n})
(fn {log10 n} {log 10 n})

;; Function to convert radians to degree and vice-versa.
(fn {to_degree x} {(* x (/ 180 pi))})
(fn {to_radian x} {(* x (/ pi 180))})