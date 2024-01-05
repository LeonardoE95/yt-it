(defun my/txt2hex (beg end)
  "opens new buffer *out* which contains the selected text as
well as the hexadecimal bytes of the text."
  (interactive
   (if (use-region-p)
       (list (region-beginning) (region-end))
     (list nil nil)))
  (if (and beg end)
      (progn
	(let ((word (buffer-substring-no-properties beg end)))
	  (switch-to-buffer-other-window "*out*")
	  (erase-buffer)	  

	  ;; -- first, for each word
	  (dotimes (i (length word))
	    ;; -- write char and hex
	    (insert (format "%c -> 0x%X\n" (aref word i) (aref word i))))
	  
	  (insert "\n")
	  
	  ;; -- then, write each hex byte in a row
	  (insert "0x")
	  (dotimes (i (length word))
	    (insert (format "%X" (aref word i))))

	  ))))
