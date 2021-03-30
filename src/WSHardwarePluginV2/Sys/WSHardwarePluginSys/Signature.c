#include "driver.h"

#define MAGIC_LENGTH 8

typedef struct _SIGNATURE_HEADER
{
	UCHAR Magic[MAGIC_LENGTH];
	ULONG SignatureLength;
	ULONG Reserved;
} SIGNATURE_HEADER, * PSIGNATURE_HEADER;

#define PUBLIC_KEY_LENGTH 72

unsigned char gPublicKey[PUBLIC_KEY_LENGTH] = {
	0x45, 0x43, 0x53, 0x31, 0x20, 0x00, 0x00, 0x00,
	0x19, 0x2D, 0x2E, 0x46, 0x1C, 0xA4, 0xE3, 0xDC,
	0xE7, 0x9D, 0x76, 0x6D, 0x4D, 0x83, 0xC2, 0x89,
	0x74, 0xBD, 0xFB, 0x66, 0x32, 0x10, 0x35, 0x3F,
	0xE3, 0xC6, 0xDF, 0xF9, 0x2B, 0xC2, 0xCE, 0xC8,
	0xE1, 0xD8, 0x10, 0x8B, 0x92, 0x33, 0xFF, 0xB7,
	0xEB, 0xEC, 0xA4, 0xB5, 0xCE, 0x72, 0x2F, 0x88,
	0x20, 0x5C, 0x65, 0x6A, 0x2A, 0x4A, 0xA7, 0x92,
	0x3A, 0x16, 0x14, 0xF8, 0x7B, 0xFE, 0xAB, 0xEB
};

UCHAR gMagic[MAGIC_LENGTH] = { 'M', 'O', 'R', 'I', 'Y', 'A' };

PSIGNATURE_HEADER LocateSignatureHeader(PUCHAR FileBuffer, ULONG FileSize)
{
	PIMAGE_DOS_HEADER DosHeader = NULL;
	PIMAGE_NT_HEADERS NtHeaders = NULL;
	PIMAGE_SECTION_HEADER FirstSectionHeader = NULL;
	PIMAGE_SECTION_HEADER LastSectionHeader = NULL;
	ULONG Length = 0;
	PSIGNATURE_HEADER SignatureHeader = NULL;

	if (FileSize < sizeof(IMAGE_DOS_HEADER))
	{
		return NULL;
	}

	DosHeader = (PIMAGE_DOS_HEADER)FileBuffer;

	if (DosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return NULL;
	}

	Length = DosHeader->e_lfanew + sizeof(IMAGE_NT_HEADERS);

	if (FileSize < Length)
	{
		return NULL;
	}

	NtHeaders = (PIMAGE_NT_HEADERS)(FileBuffer + DosHeader->e_lfanew);

	if (NtHeaders->Signature != IMAGE_NT_SIGNATURE)
	{
		return NULL;
	}

	if (IMAGE64(NtHeaders))
	{
		Length = DosHeader->e_lfanew + sizeof(IMAGE_NT_HEADERS64);

		if (FileSize < Length)
		{
			return NULL;
		}

		Length = ((PIMAGE_NT_HEADERS64)NtHeaders)->OptionalHeader.SizeOfHeaders;

		if (FileSize < Length)
		{
			return NULL;
		}

		FirstSectionHeader = (PIMAGE_SECTION_HEADER)((PIMAGE_NT_HEADERS64)NtHeaders + 1);
	}
	else
	{
		Length = ((PIMAGE_NT_HEADERS32)NtHeaders)->OptionalHeader.SizeOfHeaders;

		if (FileSize < Length)
		{
			return NULL;
		}

		FirstSectionHeader = (PIMAGE_SECTION_HEADER)((PIMAGE_NT_HEADERS32)NtHeaders + 1);
	}

	LastSectionHeader = FirstSectionHeader + NtHeaders->FileHeader.NumberOfSections - 1;

	Length = LastSectionHeader->PointerToRawData + LastSectionHeader->SizeOfRawData + sizeof(SIGNATURE_HEADER);

	if (FileSize < Length)
	{
		return NULL;
	}

	SignatureHeader = (PSIGNATURE_HEADER)(FileBuffer + LastSectionHeader->PointerToRawData + LastSectionHeader->SizeOfRawData);

	if (RtlCompareMemory(SignatureHeader->Magic, gMagic, MAGIC_LENGTH) != MAGIC_LENGTH)
	{
		return NULL;
	}

	Length += SignatureHeader->SignatureLength;

	if (FileSize < Length)
	{
		return NULL;
	}

	return SignatureHeader;
}

PUCHAR HashPeFile(PUCHAR FileBuffer, PULONG DigestLength)
{
	NTSTATUS Status = STATUS_SUCCESS;
	BCRYPT_ALG_HANDLE AlgHandle = NULL;
	PUCHAR HashObject = NULL;
	ULONG ObjectLength = 0;
	PUCHAR HashDigest = NULL;
	ULONG BytesReturn = 0;
	BCRYPT_HASH_HANDLE HashHandle = NULL;
	PIMAGE_DOS_HEADER DosHeader = NULL;
	PIMAGE_NT_HEADERS NtHeaders = NULL;
	ULONG SizeOfHeaders = 0;
	PULONG CheckSum = NULL;
	PIMAGE_DATA_DIRECTORY DirectoryEntry = NULL;
	PIMAGE_SECTION_HEADER FirstSectionHeader = NULL;
	PIMAGE_SECTION_HEADER SectionHeader = NULL;

	do
	{
		Status = BCryptOpenAlgorithmProvider(&AlgHandle, BCRYPT_SHA256_ALGORITHM, NULL, 0);

		if (!NT_SUCCESS(Status))
		{
			break;
		}

		Status = BCryptGetProperty(AlgHandle, BCRYPT_OBJECT_LENGTH, (PUCHAR)&ObjectLength, sizeof(ULONG), &BytesReturn, 0);

		if (!NT_SUCCESS(Status))
		{
			break;
		}

		Status = BCryptGetProperty(AlgHandle, BCRYPT_HASH_LENGTH, (PUCHAR)DigestLength, sizeof(ULONG), &BytesReturn, 0);

		if (!NT_SUCCESS(Status))
		{
			break;
		}

		HashObject = (PUCHAR)ExAllocatePool(NonPagedPool, ObjectLength);

		if (!HashObject)
		{
			Status = STATUS_INSUFFICIENT_RESOURCES;
			break;
		}

		RtlZeroMemory(HashObject, ObjectLength);

		HashDigest = (PUCHAR)ExAllocatePool(NonPagedPool, *DigestLength);

		if (!HashDigest)
		{
			Status = STATUS_INSUFFICIENT_RESOURCES;
			break;
		}

		RtlZeroMemory(HashDigest, *DigestLength);

		Status = BCryptCreateHash(AlgHandle, &HashHandle, HashObject, ObjectLength, NULL, 0, 0);

		if (!NT_SUCCESS(Status))
		{
			break;
		}

		DosHeader = (PIMAGE_DOS_HEADER)FileBuffer;
		NtHeaders = (PIMAGE_NT_HEADERS)(FileBuffer + DosHeader->e_lfanew);

		CheckSum = OPTIONAL_HEADER_FIELD_ADDRESS(NtHeaders, CheckSum);

		Status = BCryptHashData(HashHandle, (PUCHAR)DosHeader, (ULONG)((PUCHAR)CheckSum - (PUCHAR)DosHeader), 0);

		if (!NT_SUCCESS(Status))
		{
			break;
		}

		DirectoryEntry = &OPTIONAL_HEADER_FIELD(NtHeaders, DataDirectory)[IMAGE_DIRECTORY_ENTRY_SECURITY];

		Status = BCryptHashData(HashHandle, (PUCHAR)(CheckSum + 1), (ULONG)((PUCHAR)DirectoryEntry - (PUCHAR)(CheckSum + 1)), 0);

		if (!NT_SUCCESS(Status))
		{
			break;
		}

		SizeOfHeaders = OPTIONAL_HEADER_FIELD(NtHeaders, SizeOfHeaders);

		Status = BCryptHashData(HashHandle, (PUCHAR)(DirectoryEntry + 1), (ULONG)(FileBuffer + SizeOfHeaders - (PUCHAR)(DirectoryEntry + 1)), 0);

		if (!NT_SUCCESS(Status))
		{
			break;
		}

		if (IMAGE64(NtHeaders))
		{
			FirstSectionHeader = (PIMAGE_SECTION_HEADER)((PIMAGE_NT_HEADERS64)NtHeaders + 1);
		}
		else
		{
			FirstSectionHeader = (PIMAGE_SECTION_HEADER)((PIMAGE_NT_HEADERS32)NtHeaders + 1);
		}

		for (SectionHeader = FirstSectionHeader; SectionHeader < FirstSectionHeader + NtHeaders->FileHeader.NumberOfSections; SectionHeader++)
		{
			if (SectionHeader->SizeOfRawData == 0)
			{
				continue;
			}

			Status = BCryptHashData(HashHandle, FileBuffer + SectionHeader->PointerToRawData, SectionHeader->SizeOfRawData, 0);

			if (!NT_SUCCESS(Status))
			{
				break;
			}
		}

		if (!NT_SUCCESS(Status))
		{
			break;
		}

		Status = BCryptFinishHash(HashHandle, HashDigest, *DigestLength, 0);
	} while (FALSE);

	if (HashHandle)
	{
		BCryptDestroyHash(HashHandle);
	}

	if (HashObject)
	{
		ExFreePool(HashObject);
	}

	if (AlgHandle)
	{
		BCryptCloseAlgorithmProvider(AlgHandle, 0);
	}

	if (!NT_SUCCESS(Status))
	{
		if (HashDigest)
		{
			ExFreePool(HashDigest);
			HashDigest = NULL;
		}
	}

	return HashDigest;
}

BOOLEAN _VerifySignature(PUCHAR Signature, ULONG SignatureLength, PUCHAR HashDigest, ULONG DigestLength, PUCHAR PublicKey, ULONG KeyLength)
{
	NTSTATUS Status = STATUS_SUCCESS;
	BCRYPT_ALG_HANDLE AlgHandle = NULL;
	BCRYPT_KEY_HANDLE KeyHandle = NULL;

	do
	{
		Status = BCryptOpenAlgorithmProvider(&AlgHandle, BCRYPT_ECDSA_P256_ALGORITHM, NULL, 0);

		if (!NT_SUCCESS(Status))
		{
			break;
		}

		Status = BCryptImportKeyPair(AlgHandle, NULL, BCRYPT_ECCPUBLIC_BLOB, &KeyHandle, PublicKey, KeyLength, 0);

		if (!NT_SUCCESS(Status))
		{
			break;
		}

		Status = BCryptVerifySignature(KeyHandle, NULL, HashDigest, DigestLength, Signature, SignatureLength, 0);
	} while (FALSE);

	if (KeyHandle)
	{
		BCryptDestroyKey(KeyHandle);
	}

	if (AlgHandle)
	{
		BCryptCloseAlgorithmProvider(AlgHandle, 0);
	}

	if (!NT_SUCCESS(Status))
	{
		return FALSE;
	}

	return TRUE;
}

BOOLEAN VerifyCurrentProcessSignature()
{
	NTSTATUS Status = STATUS_SUCCESS;
	PUNICODE_STRING ProcessPath = NULL;
	WCHAR FilePathBuffer[MAX_PATH] = { L'\\', L'?', L'?', L'\\' };
	UNICODE_STRING FilePath = { 0 };
	// ULONG ReturnLength = 0;
	PUCHAR FileBuffer = NULL;
	ULONG FileSize = 0;
	PUCHAR HashDigest = NULL;
	ULONG DigestLength = 0;
	PSIGNATURE_HEADER SignatureHeader = NULL;
	PUCHAR Signature = NULL;
	BOOLEAN ReturnValue = FALSE;

	do
	{
		/*
		Status = ZwQueryInformationProcess(NtCurrentProcess(), ProcessImageFileName, NULL, 0, &ReturnLength);

		if (Status != STATUS_INFO_LENGTH_MISMATCH)
		{
			break;
		}

		ProcessPath = ExAllocatePool(NonPagedPool, ReturnLength);

		if (!ProcessPath)
		{
			break;
		}

		Status = ZwQueryInformationProcess(NtCurrentProcess(), ProcessImageFileName, ProcessPath, ReturnLength, &ReturnLength);

		if (!NT_SUCCESS(Status))
		{
			break;
		}

		FileBuffer = ReadFile(ProcessPath, &FileSize);
		*/

		ProcessPath = GetProcessPath(PsGetCurrentProcess());

		if (!ProcessPath)
		{
			break;
		}

		FilePath.Buffer = FilePathBuffer;
		FilePath.Length = sizeof(WCHAR) * 4;
		FilePath.MaximumLength = sizeof(WCHAR) * MAX_PATH;

		Status = RtlAppendUnicodeStringToString(&FilePath, ProcessPath);

		if (!NT_SUCCESS(Status))
		{
			break;
		}

		FileBuffer = ReadFile(&FilePath, &FileSize);

		if (!FileBuffer)
		{
			break;
		}

		SignatureHeader = LocateSignatureHeader(FileBuffer, FileSize);

		if (!SignatureHeader)
		{
			break;
		}

		Signature = (PUCHAR)(SignatureHeader + 1);

		HashDigest = HashPeFile(FileBuffer, &DigestLength);

		if (!HashDigest)
		{
			break;
		}

		if (_VerifySignature(Signature, SignatureHeader->SignatureLength, HashDigest, DigestLength, gPublicKey, PUBLIC_KEY_LENGTH))
		{
			ReturnValue = TRUE;
		}
	} while (FALSE);

	if (HashDigest)
	{
		ExFreePool(HashDigest);
	}

	if (FileBuffer)
	{
		ExFreePool(FileBuffer);
	}

	if (ProcessPath)
	{
		ExFreePool(ProcessPath);
	}

	return ReturnValue;
}