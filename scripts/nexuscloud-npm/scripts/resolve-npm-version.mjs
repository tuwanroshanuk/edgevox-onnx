import { execFileSync } from 'node:child_process';
import { dirname, join } from 'node:path';
import { pathToFileURL } from 'node:url';

export function nextPackageVersion(prefix, publishedVersions) {
  const escapedPrefix = prefix.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
  const versionPattern = new RegExp(`^${escapedPrefix}\\.(\\d+)$`);
  let highestRevision = -1;

  for (const version of publishedVersions) {
    const match = versionPattern.exec(String(version));
    if (!match) continue;
    highestRevision = Math.max(highestRevision, Number.parseInt(match[1], 10));
  }

  return `${prefix}.${highestRevision + 1}`;
}

function readPublishedVersions(packageName) {
  try {
    const npmArgs = ['view', packageName, 'versions', '--json'];
    const command = process.platform === 'win32' ? process.execPath : 'npm';
    if (process.platform === 'win32') {
      npmArgs.unshift(join(dirname(process.execPath), 'node_modules', 'npm', 'bin', 'npm-cli.js'));
    }
    const output = execFileSync(command, npmArgs, {
      encoding: 'utf8',
      stdio: ['ignore', 'pipe', 'pipe'],
    });
    const parsed = JSON.parse(output);
    return Array.isArray(parsed) ? parsed : parsed ? [parsed] : [];
  } catch (error) {
    const stderr = String(error?.stderr || '');
    if (stderr.includes('E404')) return [];
    throw error;
  }
}

function main() {
  const [, , packageName, prefix] = process.argv;
  if (!packageName || !prefix) {
    throw new Error('Usage: resolve-npm-version.mjs <package-name> <version-prefix>');
  }

  process.stdout.write(nextPackageVersion(prefix, readPublishedVersions(packageName)));
}

if (import.meta.url === pathToFileURL(process.argv[1]).href) {
  main();
}
