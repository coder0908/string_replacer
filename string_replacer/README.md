-r : target string : can used multiple. and , must be used at least once.
-n : new string :    Must be used once
-s : target start string : 'start string(-s)' - 'end string(-e)' will replaced by 'new string(-n)'. 
-e : @see -s
-S : 'start of file' : 'start of file(-S)' - 'first target string' will replaced by 'new string(-n)'.
-E : 'end of file' : 'last target string' - 'end fo file(-E)' will replaced by 'new string(-n)'. 
But it's priority is lower than 'count of target string that replaced(-c)'.
ex) -r str1 -c 1 -E and str1 exist over 1, -E won't used(but not error). 
-l : target list file : this file can has multiple target file's names
-t : single target file : Can used multiple time. And, it must be used at least once.
ex) -t file 1 -t file2 -t file3
-c : 'count of target string that replaced : deafault value is 1, 0 means 'all target string in file'
