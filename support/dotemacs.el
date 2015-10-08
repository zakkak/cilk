;; Enable c-mode for .cilk files
(or (assoc "\\.cilk\\'" auto-mode-alist)
    (setq auto-mode-alist (cons '("\\.cilk\\'" . c-mode) 
				auto-mode-alist)))
