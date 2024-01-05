(require 'mu4e-alert)

(setq mu4e-alert-interesting-mail-query
      (concat
       "flag:unread AND maildir:/GmailAccount/INBOX "
       "OR "
       "flag:unread AND maildir:/OutlookAccount/INBOX "
       ))

(mu4e-alert-enable-mode-line-display)

(defun refresh-mu4e-alert-mode-line ()
  (interactive)
  (mu4e~proc-kill)
  (async-shell-command "email_sync.sh")
  (mu4e-alert-enable-mode-line-display)
  )

(run-with-timer 0 60 'refresh-mu4e-alert-mode-line)
