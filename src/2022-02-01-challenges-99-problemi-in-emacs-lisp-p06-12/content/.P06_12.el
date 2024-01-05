;; P06 (OK)
;; Find out whether a list is a palindrome.
;; A palindrome can be read forward or backward; e.g. (x a m a x).

;; NOTE: a list is palindrome if its equal to its reverse.
(defun my-check-for-palindrome (l)
  (defun my-check-for-palindrome-rec (l1 l2)
    (cond ((null l1) t)
	  ((eq (car l1) (car l2))
	   (my-check-for-palindrome-rec (cdr l1) (cdr l2)))
	  (t nil)))
  (my-check-for-palindrome-rec l (reverse l)))

(my-check-for-palindrome '(x a m a x)) ;; should return t
(my-check-for-palindrome '(x a m))     ;; should return f

;; ---------------

;; P07 (OK)
;; Flatten a nested list structure.

;; Transform a list, possibly holding lists as elements into a "flat"
;; list by replacing each list with its elements (recursively).
(defun my-flatten (l)
  (cond ((null l) nil)
	((atom l) (list l))
	(t
	 (append (my-flatten (car l))
		 (my-flatten (cdr l))))))


(my-flatten '(a (b (c d) e)))  ;; should return (a b c d e)

;; ---------------

;; P08 (OK)
;; Eliminate consecutive duplicates of list elements.

;; If a list contains repeated elements they should be replaced with a
;; single copy of the element. The order of the elements should not be
;; changed.

;; Q1: is reverse O(n) ?
;; Q2: is it better to reverse at the end or to append each time? (probably reverse)
(defun my-compress (l)
  (defun my-compress-iter (l res c)
    (cond ((null l) (reverse res))
	  ((eq (car l) c)
	   (my-compress-iter (cdr l) res c))
	  (t
	   (my-compress-iter (cdr l)
			     (cons (car l) res)
			     (car l)))))
  (if (null l) nil
    (my-compress-iter (cdr l)
		      (list (car l))
		      (car l))))

(my-compress '(a a a a b c c a a d e e e e)) ;; should return (a b c a d e)

;; ---------------

;; P09 (OK)
;; Pack consecutive duplicates of list elements into sublists.

;; If a list contains repeated elements they should be placed in
;; separate sublists.

(defun my-pack (l)
  (defun my-pack-iter (l tmp res c)
    (cond ((null l)
	   (reverse (cons tmp res)))
	  ((eq (car l) c)
	   (my-pack-iter (cdr l) (cons c tmp) res c))
	  (t
	   (my-pack-iter (cdr l)
			 (list (car l))
			 (cons tmp res)
			 (car l)))))
  (if (null l) nil
    (my-pack-iter (cdr l)
		      (list (car l))
		      nil
		      (car l))))


(my-pack '(a a a a b c c a a d e e e e)) ;; should return ((a a a a) (b) (c c) (a a) (d) (e e e e))

;; ---------------

;; P10 OK
;; Run-length encoding of a list.

;; Use the result of problem P09 to implement the so-called run-length
;; encoding data compression method.
;; Consecutive duplicates of elements are encoded as lists (N E) where
;; N is the number of duplicates of the element E.

(defun my-encode (l)
  (defun my-encode-iter (l res)
    (if (null l) 
	res
      (my-encode-iter (cdr l)
		      (cons (list (length (car l)) (caar l))
			    res))))  
  (my-encode-iter (reverse (my-pack l)) nil))

(my-encode '(a a a a b c c a a d e e e e))  ;; should return ((4 A) (1 B) (2 C) (2 A) (1 D)(4 E))

;; ---------------

;; P11 OK
;; Modified run-length encoding.

;; Modify the result of problem P10 in such a way that if an element
;; has no duplicates it is simply copied into the result list. Only
;; elements with duplicates are transferred as (N E) lists.

(defun my-encode-modified (l)
  (defun my-encode-modified-iter (l res)
    (if (null l) 
	res
      (let* ((len (length (car l)))
	     (new_elem (if (eq len 1)
			   (caar l)
			 (list len (caar l)))))	
	(my-encode-modified-iter (cdr l) (cons new_elem res)))))
  (my-encode-modified-iter (reverse (my-pack l)) nil))

(my-encode-modified '(a a a a b c c a a d e e e e)) ;; should return ((4 A) B (2 C) (2 A) D (4 E))


;; ---------------

;; P12 OK
;; Decode a run-length encoded list.

;; Given a run-length code list generated as specified in problem
;; P11. Construct its uncompressed version.

(defun my-decode (l)
  
  (defun gen-list (c n res)
    (if (eq n 0)
	res
      (gen-list c (- n 1) (cons c res))))
  
  (defun my-decode-iter (l res)
    (if (null l)
	(my-flatten res)
      (let ((sublist (if (atom (car l))
			 (list (car l))
		       (gen-list (cadar l) (caar l) nil))))
	
	(my-decode-iter (cdr l)
			(cons sublist res))))) 
     
  (reverse (my-decode-iter l nil)))


(my-decode '((4 a) b (2 c) (2 a) d (4 e))) ;; should return (a a a a b c c a a d e e e e)
