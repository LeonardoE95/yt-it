;; Per calcolare la radice quadrata di x:
;;     Sia g = 1
;;     Sia precision = 0.001

;;     Ripeti fino a quando |g^2 - x| < precision:
;;        g = (g + x/g)/2

;;     Esci e ritorna g

;; ------------------

(defconst PRECISION 0.001)

;; ------------------

(defun square (x)
  (* x x))

(defun abs (x)
  (if (>= x 0)
      x
    (- x)))

(defun average (x y)
  (/ (+ x y) 2 ))

;; ------------------

(defun my-sqrt (x)  
  (defun good-enough? (approx)
    (< (abs (- (square approx) x)) PRECISION)
    )

  (defun improve (approx)
    (average approx (/ x approx)))

  (defun my-sqrt-iter (approx)
    (if (good-enough? approx)
	approx
      (my-sqrt-iter (improve approx))
      ))  
  
  (my-sqrt-iter 1.0)
  )

;; ------------------

(defun test()
  (let
      ((numbers-to-test '(1 2 3 4 5 6 7 8 9 10)))
    
    (defun test-number (x)
      (let* (
	     (my-square-root (my-sqrt x))
	     (elisp-square-root (sqrt x))
	     (difference (abs (- my-square-root
				 elisp-square-root)))
	     (output-message
	      (format
	       (concat "number=%d"
		       ", my_square_root=%.10f"
		       ", elisp_square_root=%.10f"
		       ", difference=%.10f")
	       x
	       my-square-root
	       elisp-square-root
	       difference)))
	(if (> difference PRECISION)
	    (message (concat "[ERROR]: " output-message))
	  (message (concat "[INFO]: " output-message)))))

    (defun iter-numbers (remaining-numbers)
      (if (null (car remaining-numbers))
	  nil
	(progn
	  (test-number (car remaining-numbers))
	  (iter-numbers (cdr remaining-numbers)))))

    (iter-numbers numbers-to-test)))

;; ------------------

(defun square-root ()
  (interactive)
  (message "%.10f"
	   (my-sqrt
	    (string-to-number
	     (read-string "Calcola la radice di: ")))))
