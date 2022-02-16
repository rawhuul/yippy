;; Making Functional Decleration Easy.
(let {fn}
     (lambda {a b} {let (head a)
     	     	   	(lambda (tail a) b)
}))

(fn {unpack func item}
    {eval (join (list func) item)}
)