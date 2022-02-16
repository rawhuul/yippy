;; To open new scope
(fn {local b} {
  ((lambda {_} b) ())
})

;; Unpack list for a function
(fn {unpack func item}
    {eval (join (list func) item)}
)

;; Pack list for a function
(fn {pack function & xs} {function xs})

;; Aliases
(let {curry} unpack)
(let {uncurry} pack)

;; Something like switch case, but with more power.
(fn {select & cs} {
     if (== cs nil)
     	{error "No Selection Found"}
    	{if (first (first cs)) {second (first cs)} {unpack select (tail cs)}}
})

;; Default Case
(let {whatever} true)
