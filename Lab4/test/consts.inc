;
;  MessageBox type flags
;
MB_OK                   equ 0000H
MB_OKCANCEL             equ 0001H
MB_ABORTRETRYIGNORE     equ 0002H
MB_YESNOCANCEL          equ 0003H
MB_YESNO                equ 0004H
MB_RETRYCANCEL          equ 0005H

MB_ICONHAND             equ 0010H
MB_ICONQUESTION         equ 0020H
MB_ICONEXCLAMATION      equ 0030H
MB_ICONASTERISK         equ 0040H

MB_DEFBUTTON1           equ 0000H
MB_DEFBUTTON2           equ 0100H
MB_DEFBUTTON3           equ 0200H

MB_APPLMODAL            equ 0000H
MB_SYSTEMMODAL          equ 1000H
MB_TASKMODAL            equ 2000H

MB_NOFOCUS              equ 8000H

;
;  Conventional dialog box and message box command IDs
;
IDOK     equ   1
IDCANCEL equ   2
IDABORT  equ   3
IDRETRY  equ   4
IDIGNORE equ   5
IDYES    equ   6
IDNO     equ   7
