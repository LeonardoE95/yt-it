;; Author: Leonardo Tamiano

;; MIT License

;; Copyright (c) 2024 Leonardo Tamiano

;; Permission is hereby granted, free of charge, to any person obtaining a copy
;; of this software and associated documentation files (the "Software"), to deal
;; in the Software without restriction, including without limitation the rights
;; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
;; copies of the Software, and to permit persons to whom the Software is
;; furnished to do so, subject to the following conditions:

;; The above copyright notice and this permission notice shall be included in all
;; copies or substantial portions of the Software.

;; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
;; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
;; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
;; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
;; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
;; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
;; SOFTWARE.

;; -------------------------------------------------------------------------------------

(setq yt2git/metadata-file "youtube.org")

(setq yt2git/repos
      '(

	((:name . "yt-it")
	 (:label . "IT")
	 (:private . "/home/leo/projects/FOUNDATIONS/yt-it")
	 (:public . "/home/leo/projects/PUBLIC/yt-it"))

	((:name . "yt-en")
	 (:label . "EN")
	 (:private . "/home/leo/projects/FOUNDATIONS/yt-it/tools/emacs/11-intuizioni-emacs-lisp/content")
	 (:public . "/home/leo/projects/FOUNDATIONS/yt-it/tools/emacs/11-intuizioni-emacs-lisp/content/yt-en-public")
	 )
	)
      )

(defun yt2git/get-prop (field repo)
  (assoc-default field repo))

(defun yt2git/label2txt (label)
  (assoc-default
   label
   '(
     (:title        . "TITLE")
     (:playlist     . "PLAYLIST")
     (:pubblication . "PUBBLICATION")
     (:youtube      . "YOUTUBE")
     (:public       . "PUBLIC")
     (:private      . "PRIVATE")
     (:tbd          . "TBD")
     ))
  )

;; -------------------------------------------------------------------------------------


(defun yt2git/gen-rss ()
  (interactive)
  (yt2git/gen-rss-by-repo (yt2git/select-repository))
  )

(defun yt2git/update-rss ()
  (interactive)
  (yt2git/update-rss-by-repo (yt2git/select-repository))
  )

(defun yt2git/select-repository ()
  (let* ((repo-name (ivy-read "Project: "
			      (mapcar
			       (lambda (repo) (assoc-default :name repo))
			       yt2git/repos)))
	 (repository (car (seq-filter
			   (lambda (e) (string= (assoc-default :name e) repo-name))
			   yt2git/repos))))
    repository))

;; -------------------------------------------------------------------------------------

(defun yt2git/gen-rss-by-repo (repo)
  (let* ((public (yt2git/get-prop :public repo))
	 (rss-feed (concat public "/feed/rss.xml")))
    (with-temp-buffer

      ;; initial tags
      (insert
       (concat "<rss version=\"2.0\" xmlns:atom=\"http://www.w3.org/2005/Atom\">\n\n"
	       "<channel>\n"
	       (format "<title>Leonardo Tamiano - Youtube %s </title>\n" (yt2git/get-prop :label repo))
	       "<link> https://www.youtube.com/@LT123/videos </link>\n"
	       "<description> Aggiornamenti e metadati sui video caricati </description>\n"
	       (format "<atom:link href=\"https://leonardotamiano.xyz/rss/%s.xml\" rel=\"self\" type=\"application/rss+xml\" />\n" (assoc-default :name repo))
	       "\n"))

      ;; one for each video
      (mapcar
       (lambda (video) (yt2git/gen-rss-entry repo video))
       (yt2git/videos repo))

      ;; closing tags
      (insert "</channel>\n")
      (insert "</rss>")

      (write-file rss-feed)
      )
    ))

(defun yt2git/gen-rss-entry (repo video)
  (let* ((delim "\n\n-------------------------\n\n")
	 (description
	  (with-temp-buffer
	    (insert (yt2git/gen-description-by-video repo video))
	    (beginning-of-buffer)
	    (replace-string "\n" "<br>\n")
	    (beginning-of-buffer)
	    (replace-regexp "\\(https://.*\\)<br>" "<a href=\"\\1\"> \\1 </a> <br>")
	    (beginning-of-buffer)
	    (replace-regexp "\\(http://.*\\)<br>" "<a href=\"\\1\"> \\1 </a> <br>")
	    (buffer-string))))
    (insert
     (concat
      "<item>\n"
      "<title>" (assoc-default :title video) "</title>\n"
      "<guid>" (format "https://leonardotamiano.xyz/rss/%s.xml#" (assoc-default :name repo)) (assoc-default :youtube video) "</guid>\n"
      "<link>" (format "https://youtu.be/%s" (assoc-default :youtube video)) "</link>\n"
      "<pubDate>" (yt2git/format-rss-date (assoc-default :pubblication video)) "</pubDate>\n"
      "<description>"
      (format "<![CDATA[\n%s\n]]>" description)
      "</description>\n"
      "</item>\n\n"
      ))

    )
  )

(defun yt2git/format-rss-date (custom-date)
  (format-time-string "%a, %d %b %Y 00:00:00 GMT" (date-to-time custom-date))
  )

;;
;; With this function we can generate a custom description for a
;; specific video.
;;
(defun yt2git/gen-description-by-video (repo video)
  (let* ((org-file (concat (assoc-default :private repo) "/" yt2git/metadata-file))
	 (query (concat (yt2git/label2txt :youtube) "=\"" (assoc-default :youtube video) "\""))
	 (labels (if (string= (assoc-default :label repo) "IT")
		     (list '(:ref . "RIFERIMENTI") '(:contacts . "CONTATTI"))
		   (list '(:ref . "REFERENCES") '(:contacts . "CONTACTS"))))

	 ;; First we extract the raw data from the file. To do this we
	 ;; first select the outline based on the YOUTUBE-ID, and then
	 ;; we iterate over all entries of that outline.
	 ;;
	 (raw-data (car (org-map-entries
			 (lambda ()
			   (org-map-entries
			    (lambda () `(,(org-get-heading) . ,(org-get-entry)))
			    t 'tree))
			 query
			 (list org-file))))
	 ;; 
	 ;; Then we extract description. timestamp and references from
	 ;; the various outlines.
	 ;;
	 ;; NOTE: for this to work properly we need to have this exact
	 ;; order when writing the org file. That is, first we write
	 ;; description, then timestamp and finally the references.
	 ;;
	 (raw-description (cdadr raw-data))
	 (raw-timestamp (cdaddr raw-data))
	 (raw-references (cdr (cadddr raw-data)))
	 (template (format (concat
			    "%s"
			    "\n\n-------------------------\n\n"
			    "TIMESTAMP"
			    "\n\n"
			    "%s"
			    "\n\n-------------------------\n\n"
			    ;; "REFERENCES"
			    (assoc-default :ref labels)
			    "\n\n"
			    "- Material: https://github.com/LeonardoE95/%s/tree/main/src/%s-%s\n"
			    "%s"
			    "\n\n-------------------------\n\n"
			    ;; "CONTACTS"
			    (assoc-default :contacts labels)
			    "\n\n"
			    "- Blog: https://blog.leonardotamiano.xyz/\n"
			    "- Github: https://github.com/LeonardoE95?tab=repositories\n"
			    "- Support: https://www.paypal.com/donate/?hosted_button_id=T49GUPRXALYTQ\n"
			    )
			   raw-description
			   raw-timestamp
			   (assoc-default :name repo)
			   (assoc-default :pubblication video)
			   (assoc-default :public video)
			   raw-references))
	 (full-description (with-temp-buffer
			     (insert template)
			     (beginning-of-buffer)
			     (delete-matching-lines "begin_example")
			     (delete-matching-lines "end_example")
			     (buffer-string)
			     )))
    (kill-new full-description)
    full-description
    ))



(defun yt2git/videos (repo)
  (let ((org-file (concat (assoc-default :private repo) "/" yt2git/metadata-file))
	(query (concat (yt2git/label2txt :youtube) "={.+}")))
    (with-temp-buffer
      ;;
      ;; read youtube.org and extract over all org-mode outlines and select
      ;; only those that have a YOUTUBE property assigned. From the
      ;; org-entries only extract the things we care about to construct our
      ;; ToC, which are the following values
      ;;
      ;; Title, Playlist Pubblication Date, Video
      ;;
      (setq-local entries (org-map-entries
			   (lambda ()
			     (let ((props (org-entry-properties)))
			       (list `(:title        . ,(assoc-default (yt2git/label2txt :title) props))
				     `(:playlist     . ,(assoc-default (yt2git/label2txt :playlist) props))
				     `(:pubblication . ,(assoc-default (yt2git/label2txt :pubblication) props))
				     `(:youtube      . ,(assoc-default (yt2git/label2txt :youtube) props))
				     `(:private      . ,(assoc-default (yt2git/label2txt :private) props))
				     `(:public       . ,(assoc-default (yt2git/label2txt :public) props)))
			       ))
			   query
			   (list org-file)
			   ))

      ;; do not show all entries that have not yet received a
      ;; pubblication date or that have not been published yet.
      ;;
      (setq-local entries (seq-filter
			   (lambda (e)
			     (and
			      (not (string= (assoc-default :tbd e)
					    (assoc-default :pubblication e)))
			      (org-string<= (assoc-default :pubblication e)
					    (format-time-string "%Y-%m-%d"))))
			   entries))

      ;; sorts using the pubblication date, ordering them from most
      ;; recent video to the oldest one. Notice that while sort performs
      ;; an in-place sort, the reverse function instead creates a new
      ;; list and returns the reversed list.
      (setq-local entries (reverse
			   (sort entries (lambda (a b)
					   (string< (assoc-default :pubblication a)
						    (assoc-default :pubblication b)
						    )))))
      entries
      )
    )
  )

;; -------------------------------------------------------------------------------------

(defun yt2git/update-rss-by-repo (repo)
  (let* ((public (yt2git/get-prop :public repo))
	 (rss-feed-filename (concat public "/feed/rss.xml"))
	 (rss-feed-content (with-temp-buffer
			     (insert-file-contents rss-feed-filename)
			     (buffer-string)
			     ))

	 ;; get to the insertion point for new videos (after </channel>\n\n)
	 (insertion-point (with-temp-buffer
			    (insert rss-feed-content)
			    (beginning-of-buffer)
			    (search-forward "/>\n\n")
			    (point)
			    ))

	 ;; extract latest pubDate as a reference point
	 (latest-video-date
	  (yt2git/format-custom-date
	   (with-temp-buffer
	     (insert rss-feed-content)
	     (beginning-of-buffer)
	     (search-forward "<pubDate>")
	     (buffer-substring (point) (+ (point) 29)))))

	 ;; get all videos published after that pubDate
	 (new-videos (yt2git/videos-by-date repo latest-video-date))
	 )

    ;; for each video add a new item
    (with-temp-buffer
      (insert-file-contents rss-feed-filename)
      (goto-char insertion-point)
      (mapcar
       (lambda (video) (yt2git/gen-rss-entry repo video))
       new-videos)
      (write-file rss-feed-filename)
      )
    )
  )

;; only returns videos that are returned AFTER the given date.
(defun yt2git/videos-by-date (repo date)
  (let ((org-file (concat (yt2git/get-prop :private repo) "/" yt2git/metadata-file))
	(query (concat (yt2git/label2txt :youtube) "={.+}")))
    (with-temp-buffer
      ;; 
      ;; read youtube.org and extract over all org-mode outlines and select
      ;; only those that have a YOUTUBE property assigned. From the
      ;; org-entries only extract the things we care about to construct our
      ;; ToC, which are the following values
      ;;
      ;; Title, Playlist Pubblication Date, Video
      ;;
      (setq-local entries (org-map-entries
			   (lambda ()
			     (let ((props (org-entry-properties)))
			       (list `(:title        . ,(yt2git/get-prop (yt2git/label2txt :title) props))
				     `(:playlist     . ,(yt2git/get-prop (yt2git/label2txt :playlist) props))
				     `(:pubblication . ,(yt2git/get-prop (yt2git/label2txt :pubblication) props))
				     `(:youtube      . ,(yt2git/get-prop (yt2git/label2txt :youtube) props))
				     `(:private      . ,(yt2git/get-prop (yt2git/label2txt :private) props))
				     `(:public       . ,(yt2git/get-prop (yt2git/label2txt :public) props)))
			       ))
			   query
			   (list org-file)
			   ))

      ;; do not show all entries that have not yet received a
      ;; pubblication date or that have not been published yet.
      ;;
      (setq-local entries (seq-filter
			   (lambda (e)
			     (and
			      (not (string= (yt2git/get-prop :tbd e)
					    (yt2git/get-prop :pubblication e)))			      
			      (org-string<= (yt2git/get-prop :pubblication e)
					    (format-time-string "%Y-%m-%d"))
			      ;; THIS IS THE ONLY THING WE'VE ADDED IN THIS SPECIAL VERSION. 
			      (org-string> (yt2git/get-prop :pubblication e)
					    date)
			      )
			     )
			   entries))

      ;; sorts using the pubblication date, ordering them from most
      ;; recent video to the oldest one. Notice that while sort performs
      ;; an in-place sort, the reverse function instead creates a new
      ;; list and returns the reversed list.
      (setq-local entries (reverse
			   (sort entries (lambda (a b)
					   (string< (yt2git/get-prop :pubblication a)
						    (yt2git/get-prop :pubblication b)
						    )))))
      entries
      )
    )
  )
