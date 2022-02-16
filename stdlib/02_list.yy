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

;; Get nth element of list.
(fn {nth n l} {
  if (== n 0)
    {first l}
    {nth (- n 1) (tail l)}
})

;; Get last element of list.
(fn {last l} {nth (- (len l) 1) l})

;; Returns true if element is there in list, otherwise false.
(fn {ifelem x l} {
  if (== l nil)
    {false}
    {if (== x (first l)) {true} {ifelem x (tail l)}}
})

;; map - apply the following function to a list.
(fn {map func l} {
  if (== l nil)
    {nil}
    {join (list (func (first l))) (map func (tail l))}
})

;; filter - to filter out elements of list according to functional condition
(fn {filter func l} {
  if (== l nil)
    {nil}
    {join (if (func (first l)) {head l} {nil}) (filter func (tail l))}
})

;; fold - fold a function to a list.
(fn {fold func arg1 arg2} {
    if (== arg2 nil)
       {arg1}
       {fold func (func arg1 (first arg2)) (tail arg2)}
})

