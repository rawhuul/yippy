;; Making Functional Declaration Easy.
(let {fn}
     (lambda {a b} {let (head a)
     	     	   	(lambda (tail a) b)
}))

;; Unpack list for a function
(fn {unpack func item}
    {eval (join (list func) item)}
)

;; Pack list for a function
(fn {pack function & xs} {function xs})

;; Aliases
(let {curry} unpack)
(let {uncurry} pack)
