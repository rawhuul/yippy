;; list.yy comprises of functions that works on list

;; Get length of list
(fn {len l} {
  if (== l nil)
    {0}
    {+ 1 (len (tail l))}
})

;; Get First element of list.
(fn {first l} { eval (head l) })

;; Get second element of list.
(fn {second l} { eval (head (tail l)) })

;; Get third element of list.
(fn {third l} { eval (head (tail (tail l))) })
