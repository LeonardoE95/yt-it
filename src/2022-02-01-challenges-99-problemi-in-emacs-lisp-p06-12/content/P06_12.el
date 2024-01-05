;; Author: Leonardo Tamiano

;;  ------------------------------------------

;; P06

;; Find out whether a list is a palindrome.
;; A palindrome can be read forward or backward; e.g.

;; (x a m a x).

(reverse '(1 2 3))

;; ritorna t se l1 è uguale ad l2, nil altrimenti.
(defun my-equal-list (l1 l2)
  (cond ((and (null l1) (null l2))
	 t)
	
	((or (and (null l1) (not (null l2)))
	     (and (not (null l1)) (null l2)))
	 nil)
	
	((eq (car l1) (car l2))
	 (my-equal-list (cdr l1) (cdr l2)))

	(t nil)))

(my-equal-list '(a b a) '(a b a))

(defun my-check-for-palindrome (l)
  (my-equal-list l (reverse l)))

(my-check-for-palindrome '(x a m x))

;;  ------------------------------------------

;; P07 (**) Flatten a nested list structure.

;; Transform a list, possibly holding lists as elements into a `flat'
;; list by replacing each list with its elements (recursively).


;; (my-flatten '(a (b (c d) e))) ---> (a b c d e)

(defun my-flatten (l)
  (cond ((null l) nil)
	((atom l) (list l))
	(t
	 (append (my-flatten (car l))
		 (my-flatten (cdr l))))))


(my-flatten '(a (b (c d) e)))

;;  ------------------------------------------


;; P08 - Eliminate consecutive duplicates of list elements.

;; If a list contains repeated elements they should be replaced with a
;; single copy of the element. The order of the elements should not be
;; changed.

;; (compress '(a a a a b c c a a d e e e e)) ---> (A B C A D E)

(defun my-compress (l)
  (defun my-compress-iter (l res c)
    (cond ((null l) (reverse res))
	  ((eq (car l) c)
	   (my-compress-iter (cdr l) res c))
	  (t
	   (my-compress-iter (cdr l)
			     (cons (car l) res)
			     (car l)))))

  (if (null l)
      nil
    (my-compress-iter (cdr l)
		      (list (car l))
		      (car l))))

(my-compress nil)

;;  ------------------------------------------

;; P09 - Pack consecutive duplicates of list elements into sublists.

;; (pack '(a a a a b c c a a d e e e e)) --> ((A A A A) (B) (C C) (A A) (D) (E E E E))


(defun my-pack (l)
  (defun my-pack-iter (l tmp res c)
    (cond ((null l)
	   (reverse (cons tmp res)))
	  ((eq (car l) c)
	   (my-pack-iter (cdr l)
			 (cons c tmp)
			 res
			 c))
	  (t
	   (my-pack-iter (cdr l)
			 (list (car l))
			 (cons tmp res)
			 (car l)))))
	    
  (my-pack-iter (cdr l)
		(list (car l))
		nil
		(car l)))

(my-pack '(a a a a b c c a a d e e e e))

;;  ------------------------------------------

;; P10 (*) Run-length encoding of a list

;; Use the result of problem P09 to implement the so-called run-length
;; encoding data compression method.

; Consecutive duplicates of elements are encoded as lists (N E) where
; N is the number of duplicates of the element E.

;; (encode '(a a a a b c c a a d e e e e)) --> ((4 A) (1 B) (2 C) (2 A) (1 D)(4 E))

;; ((A A A A) (B) (C C) (A A) (D) (E E E E))

(defun my-encode (l)
  (defun my-encode-rec (l res)
    (if (null l)
	(reverse res)
      (my-encode-rec (cdr l)
		     (cons
		      (list (length (car l))
			    (caar l))
		      res))))
  (my-encode-rec (my-pack l) nil))

(my-encode '(a a a a b c c a a d e e e e))

;;  ------------------------------------------

;; P11 (*) Modified run-length encoding.

(defun my-encode-modified (l)
  (defun my-encode-modified-rec (l res)
    (if (null l)
	(reverse res)
      (let* ((len (length (car l)))
	     (new_elem (if (eq len 1)
			   (caar l)
			 (list len (caar l)))))
	(my-encode-modified-rec (cdr l)
				(cons new_elem res)))))
  (my-encode-modified-rec (my-pack l) nil))

(my-encode-modified '(a a a a b c c a a d e e e e))

;; (encode-modified '(a a a a b c c a a d e e e e)) -->  ((4 A) B (2 C) (2 A) D (4 E))

;;  ------------------------------------------
;;  P12 - Decode a run-length encoded list.

;; Given a run-length code list generated as specified in problem
;; P11. Construct its uncompressed version.

(defun my-decode (l)

  (defun generate-list (c n res)
    (if (eq n 0)
	res
      (generate-list c (- n 1) (cons c res))))

  (defun my-decode-rec (l res)
    (cond ((null l) (my-flatten (reverse res)))
	  ((atom (car l))
	   (my-decode-rec (cdr l)
			   (cons (car l) res)))

	  (t
	   (my-decode-rec (cdr l)
			  (cons
			   (generate-list (cadar l)
					  (caar l)
					  nil)
			   res)))))
  (my-decode-rec l nil))


(my-decode '((4 A) B (2 C) (2 A) D (4 E)))
