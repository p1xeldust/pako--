/**
 * Written by Paul Goldstein, December 2023.
 */

#include <stdio.h>
#include <archive.h>
#include <archive_entry.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int extract_tar(const char *source, const char *destination)
{
    struct archive *a;
    struct archive_entry *entry;

    a = archive_read_new();
    archive_read_support_filter_xz(a);
    archive_read_support_format_tar(a);

    if (archive_read_open_filename(a, source, 10240) != ARCHIVE_OK)
    {
        archive_read_free(a);
        fprintf(stderr, "Can't extract: It's a broken package.\n");
        return 0;
    }

    chdir(destination);

    while (archive_read_next_header(a, &entry) == ARCHIVE_OK)
    {
        // Распаковываем символическую ссылку
        if (archive_entry_filetype(entry) == AE_IFLNK)
        {
            if (symlink(archive_entry_symlink(entry), archive_entry_pathname(entry)) != 0)
            {
                archive_read_close(a);
                archive_read_free(a);
                return 0;
            }
            continue;
        }

        // Распаковываем папку (создаем ее)
        if (archive_entry_filetype(entry) == AE_IFDIR)
        {
            if (mkdir(archive_entry_pathname(entry), S_IRWXU | S_IRWXG | S_IRWXO) != 0)
            {
                archive_read_close(a);
                archive_read_free(a);
                return 0;
            }
            continue;
        }

        FILE *file = fopen(archive_entry_pathname(entry), "wb");
        if (!file)
        {
            archive_read_close(a);
            archive_read_free(a);
            return 0;
        }

        const void *buff;
        size_t size;
        la_int64_t offset;

        while (archive_read_data_block(a, &buff, &size, &offset) == ARCHIVE_OK)
            fwrite(buff, 1, size, file);

        fclose(file);

        // Сохраняем права доступа файла
        mode_t mode = archive_entry_perm(entry);
        chmod(archive_entry_pathname(entry), mode);
    }

    archive_read_close(a);
    archive_read_free(a);
    return 1;
}

#ifdef TEST_SUITE
int main()
{
    const char *filename = "archive.tar";
    const char *output_dir = "output";
    int result = extract_tar(filename, output_dir);
    if (result != 0)
    {
        fprintf(stderr, "Extraction failed with code: %d\n", result);
        return 1;
    }
    printf("Extraction successful.\n");
    return 0;
}
#endif