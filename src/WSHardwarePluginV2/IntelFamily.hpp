#pragma once
namespace Hardware
{
	namespace CPU
	{
		namespace Intel
		{
			constexpr auto INTEL_FAM6_ANY = 0;

			constexpr auto INTEL_FAM6_CORE_YONAH = 0x0E;

			constexpr auto INTEL_FAM6_CORE2_MEROM = 0x0F;
			constexpr auto INTEL_FAM6_CORE2_MEROM_L = 0x16;
			constexpr auto INTEL_FAM6_CORE2_PENRYN = 0x17;
			constexpr auto INTEL_FAM6_CORE2_DUNNINGTON = 0x1D;

			constexpr auto INTEL_FAM6_NEHALEM = 0x1E;
			constexpr auto INTEL_FAM6_NEHALEM_G = 0x1F /* Auburndale / Havendale */;
			constexpr auto INTEL_FAM6_NEHALEM_EP = 0x1A;
			constexpr auto INTEL_FAM6_NEHALEM_EX = 0x2E;

			constexpr auto INTEL_FAM6_WESTMERE = 0x25;
			constexpr auto INTEL_FAM6_WESTMERE_EP = 0x2C;
			constexpr auto INTEL_FAM6_WESTMERE_EX = 0x2F;

			constexpr auto INTEL_FAM6_SANDYBRIDGE = 0x2A;
			constexpr auto INTEL_FAM6_SANDYBRIDGE_X = 0x2D;
			constexpr auto INTEL_FAM6_IVYBRIDGE = 0x3A;
			constexpr auto INTEL_FAM6_IVYBRIDGE_X = 0x3E;

			constexpr auto INTEL_FAM6_HASWELL = 0x3C;
			constexpr auto INTEL_FAM6_HASWELL_X = 0x3F;
			constexpr auto INTEL_FAM6_HASWELL_L = 0x45;
			constexpr auto INTEL_FAM6_HASWELL_G = 0x46;

			constexpr auto INTEL_FAM6_BROADWELL = 0x3D;
			//			constexpr auto INTEL_FAM6_BROADWELL_X = 0x47;
			constexpr auto INTEL_FAM6_BROADWELL_X = 0x4F;
			constexpr auto INTEL_FAM6_BROADWELL_D = 0x56;

			constexpr auto INTEL_FAM6_SKYLAKE_L = 0x4E;
			constexpr auto INTEL_FAM6_SKYLAKE = 0x5E;
			constexpr auto INTEL_FAM6_SKYLAKE_X = 0x55;
			constexpr auto INTEL_FAM6_KABYLAKE_L = 0x8E;
			constexpr auto INTEL_FAM6_KABYLAKE = 0x9E;

			constexpr auto INTEL_FAM6_CANNONLAKE_L = 0x66;

			constexpr auto INTEL_FAM6_ICELAKE_X = 0x6A;
			constexpr auto INTEL_FAM6_ICELAKE_D = 0x6C;
			constexpr auto INTEL_FAM6_ICELAKE = 0x7D;
			constexpr auto INTEL_FAM6_ICELAKE_L = 0x7E;
			constexpr auto INTEL_FAM6_ICELAKE_NNPI = 0x9D;

			constexpr auto INTEL_FAM6_TIGERLAKE_L = 0x8C;
			constexpr auto INTEL_FAM6_TIGERLAKE = 0x8D;

			constexpr auto INTEL_FAM6_COMETLAKE = 0xA5;
			constexpr auto INTEL_FAM6_COMETLAKE_L = 0xA6;

			/* "Small Core" Processors (Atom) */

			constexpr auto INTEL_FAM6_ATOM_BONNELL = 0x1C /* Diamondville, Pineview */;
			constexpr auto INTEL_FAM6_ATOM_BONNELL_MID = 0x26 /* Silverthorne, Lincroft */;

			constexpr auto INTEL_FAM6_ATOM_SALTWELL = 0x36 /* Cedarview */;
			constexpr auto INTEL_FAM6_ATOM_SALTWELL_MID = 0x27 /* Penwell */;
			constexpr auto INTEL_FAM6_ATOM_SALTWELL_TABLET = 0x35 /* Cloverview */;

			constexpr auto INTEL_FAM6_ATOM_SILVERMONT = 0x37 /* Bay Trail, Valleyview */;
			constexpr auto INTEL_FAM6_ATOM_SILVERMONT_D = 0x4D /* Avaton, Rangely */;
			constexpr auto INTEL_FAM6_ATOM_SILVERMONT_MID = 0x4A /* Merriefield */;

			constexpr auto INTEL_FAM6_ATOM_AIRMONT = 0x4C /* Cherry Trail, Braswell */;
			constexpr auto INTEL_FAM6_ATOM_AIRMONT_MID = 0x5A /* Moorefield */;
			constexpr auto INTEL_FAM6_ATOM_AIRMONT_NP = 0x75 /* Lightning Mountain */;

			constexpr auto INTEL_FAM6_ATOM_GOLDMONT = 0x5C /* Apollo Lake */;
			constexpr auto INTEL_FAM6_ATOM_GOLDMONT_D = 0x5F /* Denverton */;

			/* Note: the micro-architecture is "Goldmont Plus" */
			constexpr auto INTEL_FAM6_ATOM_GOLDMONT_PLUS = 0x7A /* Gemini Lake */;

			constexpr auto INTEL_FAM6_ATOM_TREMONT_D = 0x86 /* Jacobsville */;
			constexpr auto INTEL_FAM6_ATOM_TREMONT = 0x96 /* Elkhart Lake */;
			constexpr auto INTEL_FAM6_ATOM_TREMONT_L = 0x9C /* Jasper Lake */;

			/* Xeon Phi */

			constexpr auto INTEL_FAM6_XEON_PHI_KNL = 0x57 /* Knights Landing */;
			constexpr auto INTEL_FAM6_XEON_PHI_KNM = 0x85 /* Knights Mill */;

			/* Family 5 */
			constexpr auto INTEL_FAM5_QUARK_X1000 = 0x09 /* Quark X1000 SoC */;
		}
	}
}