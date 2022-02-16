;; nil, represents empty list inspired from Emacs Lisp.
(let {nil} {})

;; In yippy, Boolean values are functionally same as in C.
;; Mean true is anything other than 0.
;; And false is 0.
(let {true} 1)
(let {false} 0)

;; A temporary variable for temporary things.
(let {temp} nil)
