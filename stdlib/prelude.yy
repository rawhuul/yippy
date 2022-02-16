;; nil, represents empty list inspired from Emacs Lisp.
(let {nil} {})

;; In yippy, Boolean values are functionally same as in C.
;; Mean true is anything other than 0.
;; And false is 0.
(let {true} 1)
(let {false} 0)

;; A temporary variable for temporary things.
(let {temp} nil)

;; Making Functional Declaration Easy.
(let {fn}
     (lambda {a b} {let (head a)
     	     	   	(lambda (tail a) b)
}))

;; Fliping function, to a function.
(fn {flip func a1 a2} {func a2 a1})

(let {tell} (flip let))