;; Making Functional Decleration Easy.
(let {fn}
     (lambda {a b} {let (head a)
     	     	   	(lambda (tail a) b)
}))

