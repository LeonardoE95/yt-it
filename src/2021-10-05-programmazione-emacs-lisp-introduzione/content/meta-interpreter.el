;; Code taken and slightly modified from "The Roots of Lisp", by Paul Graham
;; -- http://www.paulgraham.com/rootsoflisp.html

(defun my-eval (e a)
  (cond
   ;; -- atom case
   ((atom e) (cadr (assoc e a)))

   ;; -- general s-expression
   ((atom (car e))
    (cond
     ;; -- primitive s-expression
     ((eq (car e) 'quote) (cadr e))     
     ((eq (car e) 'atom) (atom (my-eval (cadr e) a)))
     ((eq (car e) 'eq) (eq (my-eval (cadr e) a)
			   (my-eval (caddr e) a)))
     ((eq (car e) 'car) (car (my-eval (cadr e) a)))
     ((eq (car e) 'cdr) (cdr (my-eval (cadr e) a)))
     ((eq (car e) 'cons) (cons (my-eval (cadr e) a)
			       (my-eval (caddr e) a)))
     ((eq (car e) 'cond) (my-evcon (cdr e) a))

     ;; -- normal s-expression: take value of function to be applied
     ;; -- from environment
     ('t
      (progn
	(my-eval (cons (cadr (assoc (car e) a))
		       (cdr e))
		 a)))))
   
   ((eq (caar e) 'lambda)    
    (my-eval (caddar e)
	  (append (my-pair (cadar e)
			(my-evlis (cdr e) a))
		  a)))))

;; ----

(defun my-evcon (c a)
  (cond ((my-eval (caar c) a) (my-eval (cadar c) a))
	('t (my-evcon (cdr c) a))))

;; ----

(defun my-evlis (m a)
  (cond ((null m) '())
	('t (cons (my-eval (car m) a)
		  (my-evlis (cdr m) a)))))

;; ----

;; takes two lists of the same length and returns a list of
;; two-element lists containing successive pairs of an element from
;; each.
(defun my-pair (x y)
  (cond ((and (null x) (null y)) '())
	((and (not (atom x)) (not (atom y)))
	 (cons (list (car x) (car y))
	       (my-pair (cdr x) (cdr y))))))

;; -------------
;; USAGE EXAMPLE


(my-eval '((lambda (x) (cons '1 x)) a) '((a 2)))
(my-eval '(f '(b c)) '((f (lambda (x) (cons 'a x)))))





















;; (my-eval 'x '((x a) (y b)))
;; (my-eval '(eq 'a 'a) '())

;; (my-eval '(f '(b c)) '((f (lambda (x) (cons 'a x)))))

;; (my-eval '(lambda (x) (+ x 2))

;; (lambda (x) (+ x 2) 2)

;; (funcall (lambda (x) (+ x 2)) 2)


   ;; ((eq (caar e) 'lambda)
   ;;  (my-eval (caddar e)
   ;; 	  (append (my-pair (cadar e)
   ;; 			(my-evlis (cdr e) a))
   ;; 		  a)))))

;; (my-eval '(f '(b c)) '((f (lambda (x) (cons 'a x)))))

;; (caddar '((lambda (x) (+ x 2))))
;; (cadar '((lambda (x) (+ x 2))))

;; (my-eval '((lambda (x) (+ x 2)) a) '((a 2)))


;; (caddar '((lambda (x) (+ x 2)) 'a))
;; (cadar '((lambda (x) (+ x 2)) 'a))
;; ;; (cdr '((lambda (x) (+ x 2)) 'a))

;; (setq e '((lambda (x) (+ x 2)) a))

;; (cadar e)
;; (cdr e)

;; (my-pair (cadar e)
;; 	 (my-evlis (cdr e) '((a 2))))

;; (cons (my-eval (car m) a)
;; 		  (my-evlis (cdr m) a))))

;; (my-eval 'a '((a 2)))

;; (cons (my-eval 'a '((a 2)))
;;       '())

;; (my-eval '(cons 'a (cons 'b 'c)) '())


;; (my-eval '(f '(b c)) '((f (lambda (x) (cons 'a x)))))

;; (my-eval '((lambda (x) (cons '1 x)) a) '(a 2))

 ;; ((eq (caar e) 'lambda)    
 ;;    (my-eval (caddar e)
 ;; 	  (append (my-pair (cadar e)
 ;; 			(my-evlis (cdr e) a))
 ;; 		  a)))))


;; ---------

;; (setq e '((lambda (x) (cons '1 x)) a))
;; (setq a '((a 2)))
;; (caddar e)

;; (my-eval (caddar e)
;; 	 (append (my-pair (cadar e)
;; 			  (my-evlis (cdr e) a))
;; 		 a))

;; (my-eval '((lambda (x) (cons '1 x)) a) '((a 2)))

;; (my-eval '(f '(b c)) '((f (lambda (x) (cons 'a x)))))

;; (setq e '(f '(b c)))
;; (setq a '((f (lambda (x) (cons 'a x)))))


;; (my-eval (cons (cdr (assoc (car e) a))
;; 	       (cdr e))
;; 	 a))))

;; (cons (cadr (assoc (car e) a))
;;       (cdr e))
