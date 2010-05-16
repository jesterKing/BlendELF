
elf_timer *elf_create_timer()
{
	elf_timer *timer;

	timer = (elf_timer*)malloc(sizeof(elf_timer));
	memset(timer, 0x0, sizeof(elf_timer));
	timer->type = ELF_TIMER;

	return timer;
}

void elf_destroy_timer(elf_timer *timer)
{
	free(timer);
}

void elf_start_timer(elf_timer *timer)
{
	timer->start = elf_get_time();
}

double elf_get_elapsed_time(elf_timer *timer)
{
	if(!timer->start) return 0.0;
	return elf_get_time()-timer->start;
}

