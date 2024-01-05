;; Date: 2021/11/07
;; Made by Leonardo Tamiano

(defun int2bin (i)
  "Given an integer, returns a string which represents the binary
digits of the given integer."
  (defun int2bin-rec (i res)
    (cond ((= i 0) (concat "0" res))
	  ((= i 1) (concat "1" res))
	  ('t
	   (int2bin-rec (lsh i -1)
			 (concat (format "%s" (logand i 1))
				 res)))))
  (int2bin-rec i ""))

(defun my/txt2bin (beg end)
  "opens new buffer *out* which contains the selected text as
  well as the binary representation of the byte used to encode
  each character in the text"
  (interactive
   (if (use-region-p)
       (list (region-beginning) (region-end))
     (list nil nil)))
  (if (and beg end)
      (progn
	(let ((word (buffer-substring-no-properties beg end)))
	  (switch-to-buffer-other-window "*out*")
	  (erase-buffer)

	  ;; -- first, print word -> bin
	  (dotimes (i (length word))
	    (let* ((no-padding-bin-str (int2bin (aref word i)))
		   (padded-bin-str
		    (concat (make-string (- 8 (length no-padding-bin-str)) ?0) no-padding-bin-str)))
	      (insert (format "%c -> %s\n" (aref word i) padded-bin-str))))

	  (insert "\n")

	  ;; -- then, print each bin in a sequence
	  (dotimes (i (length word))
	    (let* ((no-padding-bin-str (int2bin (aref word i)))
		   (padded-bin-str
		    (concat (make-string (- 8 (length no-padding-bin-str)) ?0) no-padding-bin-str)))
	      (insert (format "%s " padded-bin-str))))))))
