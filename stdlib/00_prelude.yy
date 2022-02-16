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
;; It can be helpful when we want to partially evaluate
;; a function by passing it's 2nd argument as it's 1st argument.
(fn {flip func a1 a2} {func a2 a1})

;; Now, we can talk to a variable.
(let {tell} (flip let))

;; We all done that in our maths classes.
;; Something like, (f(g(x)) where f and g are functions.
;; But one thing is intresting here, we're passing g(x) as argument to another function f(x),
;; whereas g(x) is itself a function. opfn does same thing.
(fn {opfn f g x} {f (g x)})

