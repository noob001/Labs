include <ntwin32.mak>
!ifndef MIDL
MIDL            = midl
!endif

MIDLFLAGS    = /Oi /ms_ext /c_ext /cpp_cmd $(CC)

.c.obj:
  $(CC) $(cdebug) $(cflags) $(cvarsdll) /c $<


all:  ServerDcom.dll 

# Очистка
clean:
        @-del *.obj
        @-del ServerDcom.dll
        @-del ServerDcom.h
        @-del ServerDcom.lib
        @-del ServerDcom.exp
        @-del dlldata.c
        @-del ServerDcom_?.c


#Запуск компилятора MIDL для генерации .h файлов и файлов для сборки proxy/stub dll.
ServerDcom.h dlldata.c ServerDcom_p.c ServerDcom_i.c : ServerDcom.idl
    $(MIDL) $(MIDLFLAGS) ServerDcom.idl

#Сборка файлов для proxy/stub dll 
dlldata.obj : dlldata.c
    $(CC) $(cdebug) $(cflags) -DREGISTER_PROXY_DLL $(cvarsdll) /c dlldata.c

ServerDcom_i.obj : ServerDcom_i.c

ServerDcom_p.obj : ServerDcom_p.c ServerDcom.h

#Компоновка библиотеки dll
ServerDcom.dll: ServerDcom_p.obj ServerDcom_i.obj dlldata.obj ServerDcom.def
    $(link) $(linkdebug) -DLL -OUT:ServerDcom.dll -DEF:ServerDcom.def \
    ServerDcom_p.obj ServerDcom_i.obj dlldata.obj rpcrt4.lib uuid.lib kernel32.lib
