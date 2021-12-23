#ifndef __FREEBSD_HELPER_H__
#define __FREEBSD_HELPER_H__
#pragma once

#define EVENTHANDLER_PRI_PRE_FIRST   -10000
#define EVENTHANDLER_PRI_LAST        20000

#define ESRCH 3
#define ENOMEM 12
#define EINVAL 22
#define ENOTSUP 45

#define PROT_READ	0x1     /* Page can be read.  */
#define PROT_WRITE	0x2     /* Page can be written.  */
#define PROT_EXEC	0x4     /* Page can be executed.  */
#define PROT_NONE	0x0     /* Page can not be accessed.  */

#define	TRACEBUF	struct qm_trace trace;

#define	TAILQ_EMPTY(head) ((head)->tqh_first == NULL)
#define	TAILQ_FIRST(head) ((head)->tqh_first)
#define	TAILQ_NEXT(elm, field) ((elm)->field.tqe_next)






#define	TAILQ_FIRST(head)	((head)->tqh_first)

#define	QMD_TRACE_HEAD(head)

#define	TAILQ_INIT(head) do {						\
	TAILQ_FIRST((head)) = NULL;					\
	(head)->tqh_last = &TAILQ_FIRST((head));			\
	QMD_TRACE_HEAD(head);						\
} while (0)


#define	TAILQ_HEAD(name, type)								\
struct name {												\
	struct type *tqh_first;	/* first element */				\
	struct type **tqh_last;	/* addr of last next element */	\
	TRACEBUF												\
}

#define	TAILQ_ENTRY(type)											\
struct {															\
	struct type *tqe_next;	/* next element */						\
	struct type **tqe_prev;	/* address of previous next element */	\
	TRACEBUF														\
}

#define	LIST_ENTRY(type)											\
struct {															\
	struct type *le_next;	/* next element */						\
	struct type **le_prev;	/* address of previous next element */	\
}

#define	TAILQ_FOREACH(var, head, field)						\
	for ((var) = TAILQ_FIRST((head));						\
	    (var);												\
(var) = TAILQ_NEXT((var), field))

#define	QMD_TAILQ_CHECK_TAIL(head, headname)

#define	QMD_TRACE_ELEM(elem)

#define	TAILQ_INSERT_TAIL(head, elm, field) do {			\
	QMD_TAILQ_CHECK_TAIL(head, field);				\
	TAILQ_NEXT((elm), field) = NULL;				\
	(elm)->field.tqe_prev = (head)->tqh_last;			\
	*(head)->tqh_last = (elm);					\
	(head)->tqh_last = &TAILQ_NEXT((elm), field);			\
	QMD_TRACE_HEAD(head);						\
	QMD_TRACE_ELEM(&(elm)->field);					\
} while (0)

#define _countof(a) (sizeof(a)/sizeof(*(a)))

struct qm_trace {
	char * lastfile;
	int lastline;
	char * prevfile;
	int prevline;
};

size_t countof(uint8_t array);  

static inline struct thread* curthread(void) {
	struct thread* td;

	__asm__ __volatile__ ("mov %0, %%gs:0" : "=r"(td));

	return td;
}

struct lock_object {
	const char* lo_name;
	uint32_t lo_flags;
	uint32_t lo_data;
	void* lo_witness;
};

struct sx {
	struct lock_object lock_object;
	volatile uintptr_t sx_lock;
};

struct mtx {
	struct lock_object lock_object;
	volatile void* mtx_lock;
};

typedef uint64_t vm_offset_t;

struct fpu_kern_ctx;


struct auditinfo_addr {
	uint8_t useless[184];
};

struct ucred {
	uint32_t cr_ref;					// reference count		0x0000
	uint32_t cr_uid;					// effective user id	0x0004
	uint32_t cr_ruid;					// real user id			0x0008
	uint32_t useless2;					// 						0x000C
	uint32_t useless3;					//
	uint32_t cr_rgid;					// real group id
	uint32_t useless4;					//
	void *useless5;						//
	void *useless6;						//
	void *cr_prison;					// jail(2)				0x0030
	void *useless7;						//
	uint32_t useless8;					//
	void *useless9[2];					//
	void *useless10;					//
	struct auditinfo_addr cr_audit;		//
	uint32_t *cr_groups;				// groups
	uint32_t useless12;					//
};

enum uio_rw {
	UIO_READ,
	UIO_WRITE
};

enum uio_seg {
	UIO_USERSPACE,		/* from user data space */
	UIO_SYSSPACE,		/* from system space */
	UIO_USERISPACE		/* from user I space */
};

struct iovec {
	uint64_t iov_base;
	size_t iov_len;
};

TYPE_BEGIN(struct vm_map_entry, 0xC0);
	TYPE_FIELD(struct vm_map_entry *prev, 0);
	TYPE_FIELD(struct vm_map_entry *next, 8);
	TYPE_FIELD(struct vm_map_entry *left, 0x10);
	TYPE_FIELD(struct vm_map_entry *right, 0x18);
	TYPE_FIELD(vm_offset_t start, 0x20);
	TYPE_FIELD(vm_offset_t end, 0x28);
	TYPE_FIELD(vm_offset_t offset, 0x50);
	TYPE_FIELD(uint16_t prot, 0x5C);
	TYPE_FIELD(char name[32], 0x8D);
TYPE_END();

TYPE_BEGIN(struct vm_map, 0x178);
	TYPE_FIELD(struct vm_map_entry header, 0);
	TYPE_FIELD(struct sx lock, 0xB8);
	TYPE_FIELD(struct mtx system_mtx, 0xD8);
	TYPE_FIELD(int nentries, 0x100);
TYPE_END();

TYPE_BEGIN(struct vmspace, 0x250);
	TYPE_FIELD(struct vm_map vm_map, 0);
TYPE_END();

struct proc_vm_map_entry {
	char name[32];
	vm_offset_t start;
	vm_offset_t end;
	vm_offset_t offset;
	uint16_t prot;
};

TYPE_BEGIN(struct uio, 0x30);
	TYPE_FIELD(uint64_t uio_iov, 0);
	TYPE_FIELD(uint32_t uio_iovcnt, 8);
	TYPE_FIELD(uint64_t uio_offset, 0x10);
	TYPE_FIELD(uint64_t uio_resid, 0x18);
	TYPE_FIELD(uint32_t uio_segflg, 0x20);
	TYPE_FIELD(uint32_t uio_rw, 0x24);
	TYPE_FIELD(struct thread *uio_td, 0x28);
TYPE_END();

TYPE_BEGIN(struct proc, 0x800);
TYPE_FIELD(struct proc *p_forw, 0);
TYPE_FIELD(TAILQ_HEAD(,thread) p_threads, 0x10);
TYPE_FIELD(struct ucred *p_ucred, 0x40);
TYPE_FIELD(struct filedesc *p_fd, 0x48);
TYPE_FIELD(int pid, 0xB0);
TYPE_FIELD(struct vmspace *p_vmspace, 0x168);
TYPE_FIELD(char p_appid[10], 0x390);
TYPE_FIELD(char p_comm[32], 0x44C);
TYPE_END();

TYPE_BEGIN(struct sysent, 0x30);
	TYPE_FIELD(uint32_t sy_narg, 0x00);
	TYPE_FIELD(void *sy_call, 0x08);
	TYPE_FIELD(uint16_t sy_auevent, 0x10);
	TYPE_FIELD(uint64_t sy_systrace_args_func, 0x18);
	TYPE_FIELD(uint32_t sy_entry, 0x20);
	TYPE_FIELD(uint32_t sy_return, 0x24);
	TYPE_FIELD(uint32_t sy_flags, 0x28);
	TYPE_FIELD(uint32_t sy_thrcnt, 0x2C);
TYPE_END();

TYPE_BEGIN(struct sysentvec, 0x100);
	TYPE_FIELD(int sv_size, 0);
	TYPE_FIELD(struct sysent *sv_table, 0x8);
TYPE_END();

TYPE_BEGIN(struct thread, 0x3D8); // XXX: random, don't use directly without fixing it
	TYPE_FIELD(struct mtx *volatile td_lock, 0);
	TYPE_FIELD(struct proc *td_proc, 8);
	TYPE_FIELD(TAILQ_ENTRY(thread) td_plist, 0x10);
	TYPE_FIELD(int tid, 0x88);
	TYPE_FIELD(int td_pinned, 0x12C);
	TYPE_FIELD(struct ucred *td_ucred, 0x130);
	TYPE_FIELD(char td_name[32], 0x284);
	TYPE_FIELD(uint64_t td_retval[2], 0x398);
	TYPE_FIELD(uint16_t td_priority, 0x380);
TYPE_END();


#endif
