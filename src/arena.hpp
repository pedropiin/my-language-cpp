#pragma once

/*
Com muitos tipos de nós para a parse-tree e para a AST,
temos definições "circulares" de nós (nó x depende do y,
que depende do x). Então precisamos usar ponteiros. 
No entanto, com o método padrão de alocação de memória
dinâmica (new / malloc), teriamos muitos 'cache miss',
diminuindo muito a eficiência. Então uma alocação em 
arena permite uso muito menor da RAM, aumentando a
eficiência do compilador com muitos nós.
*/


class ArenaAlloc {
    public:
        inline explicit ArenaAlloc(size_t bytes) 
            : m_size(bytes)
        {
            m_buffer = static_cast<std::byte*>(malloc(m_size)); // casting void* para std::byte*
            m_arena_ptr = m_buffer;
        }

        /*
        TODO: documentação
        */
        template <typename T> inline T* alloc() {
            void* arena_ptr = m_arena_ptr;
            m_arena_ptr += sizeof(T);
            return static_cast<T*>(arena_ptr);
        }

        // deletando constructor de copia e de atribuição
        ArenaAlloc(const ArenaAlloc&) = delete;
        ArenaAlloc& operator=(const ArenaAlloc&) = delete;
        
        /*
        TODO: documentação
        */
        inline ~ArenaAlloc() {
            free(m_buffer);
        }


    private:
        size_t m_size;
        std::byte* m_buffer;
        std::byte* m_arena_ptr;
};