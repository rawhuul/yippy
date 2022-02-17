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
(fn {match & cs} {
     if (== cs nil)
     	{error "No Selection Found"}
    	{if (first (first cs)) {second (first cs)} {unpack select (tail cs)}}
})

;; To pass match statement
(let {whatever} true)

;; Just case
(fn {case x & cs} {
  if (== cs nil)
    {error "No Case Found"}
    {if (== x (first (first cs))) {second (first cs)} {
      unpack case (join (list x) (tail cs))}}
})