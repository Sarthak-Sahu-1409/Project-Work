import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'
import { resolve } from 'path'
import tsconfigPaths from 'vite-tsconfig-paths'

export default defineConfig({
  plugins: [react(), tsconfigPaths()],
  resolve: {
    alias: {
      '~': resolve(__dirname, './app'),
      '@': resolve(__dirname, './src'),
      'app': resolve(__dirname, './app'),
      'constants': resolve(__dirname, './constants'),
      'types': resolve(__dirname, './types')
    }
  }
})
