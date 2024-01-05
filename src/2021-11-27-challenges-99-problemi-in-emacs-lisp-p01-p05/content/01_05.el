;; Author
;;   Leonardo Tamiano
;; Based on
;;   https://www.ic.unicamp.br/~meidanis/courses/mc336/2006s2/funcional/L-99_Ninety-Nine_Lisp_Problems.html
;; Video in which I discuss how to implement this code
;;   TBD
;; -----------------------------------------------

;; P01 (*) Find the last box of a list.

(defun my-last (l)
  (cond
   ;; -- base cases
   ((eq l '()) '())
   ((eq (cdr l) '()) l)
   ;; recursive-step
   ('t
    (my-last (cdr l)))))

(my-last '(a b c d)) ;; (d)

;; ---

;; P02 (*) Find the last but one box of a list.

(defun my-but-last (l)
  (cond
   ;; -- base cases
   ((eq l '())
    '())
   ((eq (cdr l) '())
    '())
   ((eq (cdr l) '())
    '())
   ((eq (cddr l) '())
    l)
   ;; -- recursive step
   ('t
    (my-but-last (cdr l)))))

(my-but-last '(a b c d)) ;; (c d)

;; ---

;; P03 (*) Find the K'th element of a list.

(defun my-element-at (l k)
  ;; NOTE: if k > 0, we return '()
  (cond
   ;; -- base cases
   ((eq l '()) '())
   ((eq k 1) (car l))
   ;; -- recursive step
   ('t
    (my-element-at (cdr l) (- k 1)))))

(my-element-at '(a b c d e) -1) ;; c

;; ---

;; P04 (*) Find the number of elements of a list.

(defun my-length (l)
  (if (eq l '())
      0
    (+ 1 (my-length (cdr l)))))

(my-length '(a b c d e f g)) ;; 7

;; ---

;; P05 (*) Reverse a list.

;; NOTE: this cannot make use of tail call optimization
(defun my-reverse-1 (l)
  (if (eq l '())
      '()
    (append
     (my-reverse-1 (cdr l))
     (list (car l)))))

(my-reverse-1 '(a b c)) ;; (c b a)

;; NOTE: this can make use of tail call optimization
(defun my-reverse-2 (l)
  (defun my-reverse-2-rec (h res)
    (if (eq h '())
	res
      (my-reverse-2-rec
       (cdr h)
       (cons (car h) res))))
  (my-reverse-2-rec l '()))

(my-reverse-2 '(a b c)) ;; (c b a)
