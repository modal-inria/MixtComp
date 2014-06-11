/* Arrays tutorial: how to modify arrays
 **/

/* @page  ArraysSharingData Arrays Tutorial 3 : Sharing Data between Arrays
 *
 * @section Direct Getting direct access to the data
 *
 * All the containers in STK++ allow a direct access to the main pointer in
 * order to perform low level operations and on the other side allow to wrap
 * allocated memory.
 *
 * @code
 * // Get a constant reference on the main pointer.
 * Type const* p_data() const;
 * // Get a reference on the main pointer.
 * Type* p_data();
 * @endcode
 **/
