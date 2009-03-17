// -*- mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// --------------------------------------------------------------------------
//                   OpenMS Mass Spectrometry Framework
// --------------------------------------------------------------------------
//  Copyright (C) 2003-2009 -- Oliver Kohlbacher, Knut Reinert
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// --------------------------------------------------------------------------
// $Maintainer: Marc Sturm $
// $Authors: $
// --------------------------------------------------------------------------

#ifndef OPENMS_FORMAT_FILEHANDLER_H
#define OPENMS_FORMAT_FILEHANDLER_H

#include <OpenMS/config.h>
#include <OpenMS/FORMAT/FileTypes.h>
#include <OpenMS/FORMAT/DTAFile.h>
#include <OpenMS/FORMAT/DTA2DFile.h>
#include <OpenMS/FORMAT/MzXMLFile.h>
#include <OpenMS/FORMAT/MzMLFile.h>
#include <OpenMS/FORMAT/FeatureXMLFile.h>
#include <OpenMS/FORMAT/MzDataFile.h>
#include <OpenMS/FORMAT/MascotInfile2.h>
#include <OpenMS/FORMAT/MS2File.h>
#include <OpenMS/CONCEPT/ProgressLogger.h>

#ifdef USE_ANDIMS
#include <OpenMS/FORMAT/ANDIFile.h>
#endif

namespace OpenMS
{
	/**
		@brief Facilitates file handling by file type recognition.

		This class provides file type recognition from the file name and
		from the file content.

		It also offer a common interface to load MSExperiment data
		and allows querying for supported file types.

		@see FileTypes

		@ingroup FileIO
	*/
	class OPENMS_DLLAPI FileHandler
	{
	 public:

		/// String representations of the file types
		static const std::string NamesOfTypes[FileTypes::SIZE_OF_TYPE];

		/**
			@brief Tries to determine the file type (by name or content)

			First the type is determined from the file name.
			It this fails, the type is determined from the file content.

			@exception Exception::FileNotFound is thrown if the file is not present
		*/
		static FileTypes::Type getType(const String& filename);


		/// Determines the file type from a file name
		static FileTypes::Type getTypeByFileName(const String& filename);

		/**
			@brief Determines the file type of a file by parsing the first few lines

			@exception Exception::FileNotFound is thrown if the file is not present
		*/
		static FileTypes::Type getTypeByContent(const String& filename);

		/// Converts a file type name into a Type
		static FileTypes::Type nameToType(const String& name);

		/// Converts a Type into a file type name
		static String typeToName(FileTypes::Type type);

		/// Returns if the file type is supported in this build of the library
		static bool isSupported(FileTypes::Type type);

    /// Mutable access to the options for loading
    PeakFileOptions& getOptions();

    /// Non-mutable access to the options for loading
    const PeakFileOptions& getOptions() const;

		/**
			 @brief Loads a file into an MSExperiment

			 @param filename the Filename of the file to load.
			 @param exp The MSExperiment to load the data into.
			 @param force_type Forces to load the file with that file type. If no type is forced, it is determined from the extention ( or from the content if that fails).
			 @param log Progress logging mode

			 @return true if the file could be loaded, false otherwise
		*/
		template <class PeakType> bool loadExperiment(const String& filename, MSExperiment<PeakType>& exp, FileTypes::Type force_type = FileTypes::UNKNOWN, ProgressLogger::LogType log = ProgressLogger::NONE)
		{
			//determine file type
			FileTypes::Type type;
			if (force_type != FileTypes::UNKNOWN)
			{
				type = force_type;
			}
			else
			{
				try
				{
					type = getType(filename);
				}
				catch(Exception::FileNotFound)
				{
					return false;
				}
			}

			//load right file
			switch(type)
			{
				case FileTypes::DTA:
					exp.reset();
					exp.resize(1);
					DTAFile().load(filename,exp[0]);
					return true;
					break;
				case FileTypes::DTA2D:
					{
						DTA2DFile f;
						f.getOptions() = options_;
						f.setLogType(log);
						f.load(filename,exp);
						return true;
					}
					break;
				case FileTypes::MZXML:
					{
						MzXMLFile f;
						f.getOptions() = options_;
						f.setLogType(log);
						f.load(filename,exp);
						return true;
					}
					break;
				case FileTypes::MZDATA:
					{
						MzDataFile f;
						f.getOptions() = options_;
						f.setLogType(log);
						f.load(filename,exp);
						return true;
					}
					break;
				case FileTypes::MZML:
					{
						MzMLFile f;
						f.getOptions() = options_;
						f.setLogType(log);
						f.load(filename,exp);
						return true;
					}
					break;
#ifdef USE_ANDIMS
				case FileTypes::ANDIMS:
					{
						ANDIFile f;
						f.setLogType(log);
						f.load(filename,exp);
						return true;
					}
					break;
#endif
				case FileTypes::MGF:
					{
						MascotInfile2 f;
						f.setLogType(log);
						f.load(filename, exp);
						return true;
					}
				case FileTypes::MS2:
					{
						MS2File f;
						f.setLogType(log);
						f.load(filename, exp);
						return true;
					}
				default:
					break;
			}
			
			return false;
		}

		/**
			 @brief Loads a file into a FeatureMap

			 @param filename the Filename of the file to load.
			 @param map The FeatureMap to load the data into.
			 @param force_type Forces to load the file with that file type. If no type is forced, it is determined from the extention ( or from the content if that fails).

			 @return true if the file could be loaded, false otherwise
		*/
		template <class FeatureType> bool loadFeatures(const String& filename, FeatureMap<FeatureType>& map, FileTypes::Type force_type = FileTypes::UNKNOWN)
		{
			//determine file type
			FileTypes::Type type;
			if (force_type != FileTypes::UNKNOWN)
			{
				type = force_type;
			}
			else
			{
				try
				{
					type = getType(filename);
				}
				catch(Exception::FileNotFound)
				{
					return false;
				}
			}

			//load right file
			if (type==FileTypes::FEATUREXML)
			{
				FeatureXMLFile().load(filename,map);
				return true;
			}
			
			return false;
		}

		private:
		  PeakFileOptions options_;
	};

} //namespace

#endif //OPENMS_FORMAT_FILEHANDLER_H
