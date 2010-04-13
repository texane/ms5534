##
## Made by fabien le mentec <texane@gmail.com>
## 
## Started on  Sun Sep 20 00:28:27 2009 texane
## Last update Tue Apr 13 10:45:45 2010 texane
##


PATH	:=	$(PATH):/usr/share/pk2

RM	:=	-rm
MV	:=	mv

CC	:=	/home/texane/proj/present/pic/18f4550/tools/sdcc/bin/sdcc
CFLAGS	:=	-mpic16						\
		--model-large					\
		-p18f4550 -Dpic18f4550				\
		-DSDCC_PIC18					\
		-I../src					\
		-I../../puf/include

AS	:=	gpasm
ASFLAGS	:=	-q -p18f4550 -Dpic18f4550 			\
		-I /usr/share/gputils/header 			\
		-DSDCC

LD	:=	$(CC)
LDFLAGS	:=	--use-crt=crt0.o				\
		-Wl-s../src/template-18f4550.lkr,-m		\
		-mpic16 -p18f4550
LIBS	:=	-llibdev18f4550.lib -llibsdcc.lib -llibc18f.lib

LIB	:=	gplib -c

NAME	:=	ms5534.hex

CSRCS	:=	../src/main.c 			\
		../src/ms5534.c			\
		../src/config.c
ASRCS	:=

OBJS	:=	$(CSRCS:.c=.o)			\
		$(ASRCS:.asm=.o)

GARBAGE	:=	$(NAME:.hex=.lst)		\
		$(NAME:.hex=.cod)		\
		$(CSRCS:.c=.lst)		\
		$(CSRCS:.c=.asm)

# rules

all	:	$(NAME)

$(NAME)	:	$(OBJS)
		$(LD) $(OBJS) -o $(NAME) $(LDFLAGS) $(LIBS)

%.o	:	%.c
		$(CC) $(CFLAGS) -c -o $(@) $(<)

%.o	:	%.asm
		$(AS) $(ASFLAGS) -c -o $(@) $(<)

PHONY	:	clean fclean re write erase test

clean	:
		$(RM) $(OBJS)
		$(RM) $(GARBAGE)

fclean	:	clean
		$(RM) $(NAME)
		$(RM) m600.map

re	:	fclean all

write	:	$(NAME)
		pk2cmd -PPIC18f4550 -M -F$(NAME)
