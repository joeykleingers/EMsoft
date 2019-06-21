! ###################################################################
! Copyright (c) 2014-2019, Marc De Graef Research Group/Carnegie Mellon University
! All rights reserved.
!
! Redistribution and use in source and binary forms, with or without modification, are
! permitted provided that the following conditions are met:
!
!     - Redistributions of source code must retain the above copyright notice, this list
!        of conditions and the following disclaimer.
!     - Redistributions in binary form must reproduce the above copyright notice, this
!        list of conditions and the following disclaimer in the documentation and/or
!        other materials provided with the distribution.
!     - Neither the names of Marc De Graef, Carnegie Mellon University nor the names
!        of its contributors may be used to endorse or promote products derived from
!        this software without specific prior written permission.
!
! THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
! AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
! IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
! ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
! LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
! DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
! SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
! CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
! OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
! USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
! ###################################################################

!--------------------------------------------------------------------------
! EMsoft:configmod.f90
!--------------------------------------------------------------------------
!
! MODULE: configmod
!
!> @author Marc De Graef, Carnegie Mellon University
!
!> @brief simple module to convert an array of C strings into the EMsoft configuration parameters
!
!> @date 10/28/17 MDG 1.0 original
!> @date 03/29/18 MDG 1.1 removed all stdout usage
!--------------------------------------------------------------------------
module configmod

use local
use io
use, intrinsic :: iso_c_binding

IMPLICIT NONE


public :: print_EMsoft_configuration_strings, C2F_configuration_strings

private :: cstrlen ! we keep this private to make sure there are no conflicts with other packages

contains

!--------------------------------------------------------------------------
!
! SUBROUTINE: print_EMsoft_configuration_strings
!
!> @author Marc De Graef, Carnegie Mellon University
!
!> @brief prints the entire ConfigStructureType content
!
!> @date 10/28/17 MDG 1.0 original
!--------------------------------------------------------------------------
subroutine print_EMsoft_configuration_strings(CP, fname)
!DEC$ ATTRIBUTES DLLEXPORT :: print_EMsoft_configuration_strings

IMPLICIT NONE

type(ConfigStructureType),INTENT(IN)   :: CP
character(fnlen),OPTIONAL              :: fname

integer(kind=irg)                      :: ii

if (PRESENT(fname)) then
    open(unit=23,file=trim(fname),status='unknown')
    write (23,*) 'EMsoft Configuration Strings'
    write (23,*) '----------------------------'

    write (23,*) trim(ConfigStructureNames(1))//' : '//trim(CP%EMsoftpathname) 
    write (23,*) trim(ConfigStructureNames(2))//' : '//trim(CP%EMXtalFolderpathname) 
    write (23,*) trim(ConfigStructureNames(3))//' : '//trim(CP%EMdatapathname)
    write (23,*) trim(ConfigStructureNames(4))//' : '//trim(CP%EMtmppathname) 
    write (23,*) trim(ConfigStructureNames(5))//' : '//trim(CP%EMsoftLibraryLocation) 
    write (23,*) trim(ConfigStructureNames(6))//' : '//trim(CP%EMSlackWebHookURL) 
    write (23,*) trim(ConfigStructureNames(7))//' : '//trim(CP%EMSlackChannel) 
    write (23,*) trim(ConfigStructureNames(8))//' : '//trim(CP%UserName) 
    write (23,*) trim(ConfigStructureNames(9))//' : '//trim(CP%UserLocation) 
    write (23,*) trim(ConfigStructureNames(10))//' : '//trim(CP%UserEmail) 
    write (23,*) trim(ConfigStructureNames(11))//' : '//trim(CP%EMNotify)
    write (23,*) trim(ConfigStructureNames(12))//' : '//trim(CP%Develop) 
    write (23,*) trim(ConfigStructureNames(13))//' : '//trim(CP%Release) 
    write (23,*) trim(ConfigStructureNames(14))//' : '//trim(CP%h5copypath) 
    write (23,*) trim(ConfigStructureNames(15))//' : '//trim(CP%EMsoftplatform) 
    write (23,*) trim(ConfigStructureNames(16))//' : '//trim(CP%EMsofttestpath) 
    write (23,*) trim(ConfigStructureNames(17))//' : '//trim(CP%EMsoftTestingPath) 
    write (23,*) trim(ConfigStructureNames(18))//' : '//trim(CP%EMsoftversion) 
    write (23,*) trim(ConfigStructureNames(19))//' : '//trim(CP%Configpath) 
    write (23,*) trim(ConfigStructureNames(20))//' : '//trim(CP%Templatepathname) 
    write (23,*) trim(ConfigStructureNames(21))//' : '//trim(CP%Resourcepathname) 
    write (23,*) trim(ConfigStructureNames(22))//' : '//trim(CP%Homepathname) 
    write (23,*) trim(ConfigStructureNames(23))//' : '//trim(CP%OpenCLpathname) 
    write (23,*) trim(ConfigStructureNames(24))//' : '//trim(CP%Templatecodefilename) 
    write (23,*) trim(ConfigStructureNames(25))//' : '//trim(CP%WyckoffPositionsfilename) 
    write (23,*) trim(ConfigStructureNames(26))//' : '//trim(CP%Randomseedfilename) 
    write (23,*) trim(ConfigStructureNames(27))//' : '//trim(CP%EMsoftnativedelimiter) 
    write (23,*) trim(ConfigStructureNames(29))//' : '//trim(CP%maskfile) 
    write (23,*) trim(ConfigStructureNames(30))//' : '//trim(CP%scalingmode) 
    write (23,*) trim(ConfigStructureNames(31))//' : '//trim(CP%exptfile) 
    write (23,*) trim(ConfigStructureNames(32))//' : '//trim(CP%HDFstrings(1)) 
    write (23,*) trim(ConfigStructureNames(33))//' : '//trim(CP%HDFstrings(2)) 
    write (23,*) trim(ConfigStructureNames(34))//' : '//trim(CP%HDFstrings(3)) 
    write (23,*) trim(ConfigStructureNames(35))//' : '//trim(CP%HDFstrings(4)) 
    write (23,*) trim(ConfigStructureNames(36))//' : '//trim(CP%HDFstrings(5)) 
    write (23,*) trim(ConfigStructureNames(37))//' : '//trim(CP%HDFstrings(6)) 
    write (23,*) trim(ConfigStructureNames(38))//' : '//trim(CP%HDFstrings(7)) 
    write (23,*) trim(ConfigStructureNames(39))//' : '//trim(CP%HDFstrings(8)) 
    write (23,*) trim(ConfigStructureNames(40))//' : '//trim(CP%HDFstrings(9)) 
    write (23,*) trim(ConfigStructureNames(41))//' : '//trim(CP%HDFstrings(10)) 
    write (23,*) trim(ConfigStructureNames(42))//' : '//trim(CP%tmpfile) 
    write (23,*) trim(ConfigStructureNames(43))//' : '//trim(CP%datafile) 
    write (23,*) trim(ConfigStructureNames(44))//' : '//trim(CP%ctffile) 
    write (23,*) trim(ConfigStructureNames(45))//' : '//trim(CP%avctffile) 
    write (23,*) trim(ConfigStructureNames(46))//' : '//trim(CP%angfile) 
    write (23,*) trim(ConfigStructureNames(47))//' : '//trim(CP%eulerfile) 
    write (23,*) trim(ConfigStructureNames(48))//' : '//trim(CP%dictfile) 
    write (23,*) trim(ConfigStructureNames(49))//' : '//trim(CP%masterfile) 
    write (23,*) trim(ConfigStructureNames(50))//' : '//trim(CP%indexingmode) 
    close(unit=23,status='keep')
else

    call Message('EMsoft Configuration Strings')
    call Message('----------------------------')

    call Message(trim(ConfigStructureNames(1))//' : '//trim(CP%EMsoftpathname)) 
    call Message(trim(ConfigStructureNames(2))//' : '//trim(CP%EMXtalFolderpathname)) 
    call Message(trim(ConfigStructureNames(3))//' : '//trim(CP%EMdatapathname)) 
    call Message(trim(ConfigStructureNames(4))//' : '//trim(CP%EMtmppathname)) 
    call Message(trim(ConfigStructureNames(5))//' : '//trim(CP%EMsoftLibraryLocation)) 
    call Message(trim(ConfigStructureNames(6))//' : '//trim(CP%EMSlackWebHookURL)) 
    call Message(trim(ConfigStructureNames(7))//' : '//trim(CP%EMSlackChannel)) 
    call Message(trim(ConfigStructureNames(8))//' : '//trim(CP%UserName)) 
    call Message(trim(ConfigStructureNames(9))//' : '//trim(CP%UserLocation)) 
    call Message(trim(ConfigStructureNames(10))//' : '//trim(CP%UserEmail)) 
    call Message(trim(ConfigStructureNames(11))//' : '//trim(CP%EMNotify)) 
    call Message(trim(ConfigStructureNames(12))//' : '//trim(CP%Develop)) 
    call Message(trim(ConfigStructureNames(13))//' : '//trim(CP%Release)) 
    call Message(trim(ConfigStructureNames(14))//' : '//trim(CP%h5copypath)) 
    call Message(trim(ConfigStructureNames(15))//' : '//trim(CP%EMsoftplatform)) 
    call Message(trim(ConfigStructureNames(16))//' : '//trim(CP%EMsofttestpath)) 
    call Message(trim(ConfigStructureNames(17))//' : '//trim(CP%EMsoftTestingPath)) 
    call Message(trim(ConfigStructureNames(18))//' : '//trim(CP%EMsoftversion)) 
    call Message(trim(ConfigStructureNames(19))//' : '//trim(CP%Configpath)) 
    call Message(trim(ConfigStructureNames(20))//' : '//trim(CP%Templatepathname)) 
    call Message(trim(ConfigStructureNames(21))//' : '//trim(CP%Resourcepathname)) 
    call Message(trim(ConfigStructureNames(22))//' : '//trim(CP%Homepathname)) 
    call Message(trim(ConfigStructureNames(23))//' : '//trim(CP%OpenCLpathname)) 
    call Message(trim(ConfigStructureNames(24))//' : '//trim(CP%Templatecodefilename)) 
    call Message(trim(ConfigStructureNames(25))//' : '//trim(CP%WyckoffPositionsfilename)) 
    call Message(trim(ConfigStructureNames(26))//' : '//trim(CP%Randomseedfilename)) 
    call Message(trim(ConfigStructureNames(27))//' : '//trim(CP%EMsoftnativedelimiter)) 
    call Message(trim(ConfigStructureNames(29))//' : '//trim(CP%maskfile))
    call Message(trim(ConfigStructureNames(30))//' : '//trim(CP%scalingmode)) 
    call Message(trim(ConfigStructureNames(31))//' : '//trim(CP%exptfile))
    call Message(trim(ConfigStructureNames(32))//' : '//trim(CP%HDFstrings(1)))
    call Message(trim(ConfigStructureNames(33))//' : '//trim(CP%HDFstrings(2)))
    call Message(trim(ConfigStructureNames(34))//' : '//trim(CP%HDFstrings(3)))
    call Message(trim(ConfigStructureNames(35))//' : '//trim(CP%HDFstrings(4)))
    call Message(trim(ConfigStructureNames(36))//' : '//trim(CP%HDFstrings(5)))
    call Message(trim(ConfigStructureNames(37))//' : '//trim(CP%HDFstrings(6)))
    call Message(trim(ConfigStructureNames(38))//' : '//trim(CP%HDFstrings(7)))
    call Message(trim(ConfigStructureNames(39))//' : '//trim(CP%HDFstrings(8)))
    call Message(trim(ConfigStructureNames(40))//' : '//trim(CP%HDFstrings(9)))
    call Message(trim(ConfigStructureNames(41))//' : '//trim(CP%HDFstrings(10)))
    call Message(trim(ConfigStructureNames(42))//' : '//trim(CP%tmpfile))
    call Message(trim(ConfigStructureNames(43))//' : '//trim(CP%datafile))
    call Message(trim(ConfigStructureNames(44))//' : '//trim(CP%ctffile))
    call Message(trim(ConfigStructureNames(45))//' : '//trim(CP%avctffile))
    call Message(trim(ConfigStructureNames(46))//' : '//trim(CP%angfile))
    call Message(trim(ConfigStructureNames(47))//' : '//trim(CP%eulerfile))
    call Message(trim(ConfigStructureNames(48))//' : '//trim(CP%dictfile))
    call Message(trim(ConfigStructureNames(49))//' : '//trim(CP%masterfile))
    call Message(trim(ConfigStructureNames(50))//' : '//trim(CP%indexingmode))
end if

end subroutine print_EMsoft_configuration_strings

!--------------------------------------------------------------------------
!
! SUBROUTINE: C2F_configuration_strings
!
!> @author Marc De Graef, Carnegie Mellon University
!
!> @brief Converts string array to EMsoft ConfigStructureType 
!
!> @date 10/28/17 MDG 1.0 original
!> @date 01/22/18 MDG 1.1 added strvals array
!--------------------------------------------------------------------------
subroutine C2F_configuration_strings(cptr, CS) ! bind(C)
!DEC$ ATTRIBUTES DLLEXPORT :: C2F_configuration_strings

IMPLICIT NONE

type(c_ptr), INTENT(IN), value        :: cptr
type(ConfigStructureType),INTENT(OUT) :: CS

character(kind=c_char), pointer       :: fptr(:,:)
integer(kind=irg)               	  :: ii, lenstr
character(fnlen)                      :: blank

call c_f_pointer(cptr, fptr, [ fnlen, wraparraysize ])

CS%EMsoftpathname = cstrf(fptr(1:fnlen,1))
CS%EMXtalFolderpathname = cstrf(fptr(1:fnlen,2))
CS%EMdatapathname = cstrf(fptr(1:fnlen,3))
CS%EMtmppathname = cstrf(fptr(1:fnlen,4))
CS%EMsoftLibraryLocation = cstrf(fptr(1:fnlen,5))
CS%EMSlackWebHookURL = cstrf(fptr(1:fnlen,6))
CS%EMSlackChannel = cstrf(fptr(1:fnlen,7))
CS%UserName = cstrf(fptr(1:fnlen,8))
CS%UserLocation = cstrf(fptr(1:fnlen,9))
CS%UserEmail = cstrf(fptr(1:fnlen,10))
CS%EMNotify = cstrf(fptr(1:fnlen,11))
CS%Develop = cstrf(fptr(1:fnlen,12))
CS%Release = cstrf(fptr(1:fnlen,13))
CS%h5copypath = cstrf(fptr(1:fnlen,14))
CS%EMsoftplatform = cstrf(fptr(1:fnlen,15))
CS%EMsofttestpath = cstrf(fptr(1:fnlen,16))
CS%EMsoftTestingPath = cstrf(fptr(1:fnlen,17))
CS%EMsoftversion = cstrf(fptr(1:fnlen,18))
CS%Configpath = cstrf(fptr(1:fnlen,19))
CS%Templatepathname = cstrf(fptr(1:fnlen,20))
CS%Resourcepathname = cstrf(fptr(1:fnlen,21))
CS%Homepathname = cstrf(fptr(1:fnlen,22))
CS%OpenCLpathname = cstrf(fptr(1:fnlen,23))
CS%Templatecodefilename = cstrf(fptr(1:fnlen,24))
CS%WyckoffPositionsfilename = cstrf(fptr(1:fnlen,25))
CS%Randomseedfilename = cstrf(fptr(1:fnlen,26))
CS%EMsoftnativedelimiter = cstrf(fptr(1:fnlen,27))
CS%maskfile = cstrf(fptr(1:fnlen,29))
CS%scalingmode = cstrf(fptr(1:fnlen,30))
CS%exptfile = cstrf(fptr(1:fnlen,31))
CS%HDFstrings(1) = cstrf(fptr(1:fnlen,32))
CS%HDFstrings(2) = cstrf(fptr(1:fnlen,33))
CS%HDFstrings(3) = cstrf(fptr(1:fnlen,34))
CS%HDFstrings(4) = cstrf(fptr(1:fnlen,35))
CS%HDFstrings(5) = cstrf(fptr(1:fnlen,36))
CS%HDFstrings(6) = cstrf(fptr(1:fnlen,37))
CS%HDFstrings(7) = cstrf(fptr(1:fnlen,38))
CS%HDFstrings(8) = cstrf(fptr(1:fnlen,39))
CS%HDFstrings(9) = cstrf(fptr(1:fnlen,40))
CS%HDFstrings(10) = cstrf(fptr(1:fnlen,41))
CS%tmpfile = cstrf(fptr(1:fnlen,42))
CS%datafile = cstrf(fptr(1:fnlen,43))
CS%ctffile = cstrf(fptr(1:fnlen,44))
CS%avctffile = cstrf(fptr(1:fnlen,45))
CS%angfile = cstrf(fptr(1:fnlen,46))
CS%eulerfile = cstrf(fptr(1:fnlen,47))
CS%dictfile = cstrf(fptr(1:fnlen,48))
CS%masterfile = cstrf(fptr(1:fnlen,49))
CS%indexingmode = cstrf(fptr(1:fnlen,50))

! also copy all the strings into a regular array of strings; this is useful
! if we want to extract strings that do not correspond to any of the predefined
! strings in the list above.

do ii=1,wraparraysize
  CS%strvals(ii) = cstrf(fptr(1:fnlen,ii))
end do 

end subroutine C2F_configuration_strings


!--------------------------------------------------------------------------
!
! FUNCTION: cstrlen
!
!> @author Marc De Graef, Carnegie Mellon University
!
!> @brief compute the length of a null-terminated C string
!
!> @date 10/28/17 MDG 1.0 original
!--------------------------------------------------------------------------
function cstrlen(carray) result(res)

IMPLICIT NONE

character(kind=c_char), INTENT(IN) :: carray(:)
integer(kind=irg)                  :: res

integer(kind=irg)                  :: ii

res = size(carray)
do ii = 1, size(carray)
  if (carray(ii) == C_NULL_CHAR) then
    res = ii - 1
    return
  end if
end do

end function cstrlen

!--------------------------------------------------------------------------
!
! FUNCTION: cstrf
!
!> @author Marc De Graef, Carnegie Mellon University
!
!> @brief convert a c string into a correctly formatted f string
!
!> @date 11/30/17 MDG 1.0 original
!--------------------------------------------------------------------------
function cstrf(carray) result(res)

IMPLICIT NONE

character(kind=c_char), INTENT(IN) :: carray(:)
character(fnlen)                   :: res

integer(kind=irg)                  :: ii, lenstr

lenstr = cstrlen(carray)
res = ''
do ii = 1, lenstr
    res(ii:ii) = carray(ii)
end do

end function cstrf



end module configmod
