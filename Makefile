# Makefile for kld char device driver.
# $FreeBSD: release/9.0.0/share/examples/kld/cdev/module/Makefile 118385 2003-08-03 10:39:29Z mbr $

KMOD=	seq
SRCS=	seq.c

.include <bsd.kmod.mk>
